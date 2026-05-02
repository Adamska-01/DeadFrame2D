#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/ECS/System/Scene/Scene.h"
#include "Engine/Events/GameComponent/GameComponentAddedEvent.h"
#include "Engine/Events/GameObject/GameObjectCreatedEvent.h"
#include "Engine/Events/GameObject/GameObjectDestroyedEvent.h"
#include "Engine/Events/GameObject/GameObjectHierarchyChangeEvent.h"
#include <algorithm>
#include <cassert>


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	Scene::Scene()
	{
		isRunning = false;

		Exit();

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this, &Scene::GameObjectDestroyedHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectHierarchyChangeEvent)), this, &Scene::GameObjectHierarchyChangeHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameComponentAddedEvent)), this, &Scene::GameComponentAddedHandler);
	}

	Scene::~Scene()
	{
		Exit();

		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectHierarchyChangeEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameComponentAddedEvent)), this);
	}

	uint32_t Scene::FindFreeSlot()
	{
		if (!freeSlots.empty())
		{
			auto index = freeSlots.back();

			freeSlots.pop_back();

			return index;
		}

		entries.emplace_back();

		return static_cast<uint32_t>(entries.size() - 1);
	}

	GameObject* Scene::GetAt(uint32_t index) const
	{
		assert(index < entries.size());

		return entries[index].object.get();
	}

	bool Scene::IsValid(uint32_t index, uint32_t generation) const
	{
		if (index >= entries.size()) 
			return false;
		
		const auto& e = entries[index];
		
		return e.state == ObjectEntryState::ALIVE && e.generation == generation;
	}

	void Scene::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

		if (!gameObjEvent || gameObjEvent->gameObjectDestroyed == nullptr)
			return;

		auto entryIndex = gameObjEvent->gameObjectDestroyed.GetIndex();

		// Assert the index
		if (GetAt(entryIndex) == nullptr)
			return;

		objectsPendingDestruction.push_back(entryIndex);

		// Mark for destruction immediately (invalidate handle)
		auto& entry = entries[entryIndex];
		entry.state = ObjectEntryState::DESTRUCTION_PENDING;
	}

	void Scene::GameObjectHierarchyChangeHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto hierarchyChangeEvent = DispatchableEvent::SafeCast<GameObjectHierarchyChangeEvent>(dispatchableEvent);

		if (hierarchyChangeEvent == nullptr)
			return;

		auto& targetObj = hierarchyChangeEvent->GetObject();
		auto& oldParent = hierarchyChangeEvent->GetOldParent();
		auto& newParent = hierarchyChangeEvent->GetNewParent();

		if (!targetObj)
			return;

		// was root -> now child
		if (oldParent == nullptr && newParent != nullptr)
		{
			gameObjectRoots.erase(
				std::remove(
					gameObjectRoots.begin(),
					gameObjectRoots.end(),
					targetObj.GetIndex()),
				gameObjectRoots.end());
		}
		// was child -> now root
		else if (oldParent != nullptr && newParent == nullptr)
		{
			auto findIT = std::find(
				gameObjectRoots.begin(),
				gameObjectRoots.end(),
				targetObj.GetIndex());

			if (findIT == gameObjectRoots.end())
			{
				gameObjectRoots.push_back(targetObj.GetIndex());
			}
		}
	}

	void Scene::GameComponentAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameComponentAddedEvent = DispatchableEvent::SafeCast<GameComponentAddedEvent>(dispatchableEvent);

		if (gameComponentAddedEvent == nullptr)
			return;

		auto newComponent = gameComponentAddedEvent->GetGameComponentHandle();

		if (newComponent && isRunning)
		{
			newComponent->Init();
			newComponent->Start();
		}
	}

	void Scene::SendGameObjectCreatedEvent(const ObjectHandle<GameObject>& obj)
	{
		EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(obj));
	}

	void Scene::ProcessPendingDestructions()
	{
		for (const auto& toDestroyIndex : objectsPendingDestruction)
		{
			auto& entry = entries[toDestroyIndex];
			auto* obj = entry.object.get();

			if (obj == nullptr)
				continue;

			// Remove from roots
			if (obj->GetParent() == nullptr)
			{
				gameObjectRoots.erase(
					std::remove(
						gameObjectRoots.begin(),
						gameObjectRoots.end(),
						toDestroyIndex),
					gameObjectRoots.end());
			}

			// Final destruction
			entry.state = ObjectEntryState::DEAD;
			entry.generation++;
			entry.object.reset();

			freeSlots.push_back(toDestroyIndex);
		}

		objectsPendingDestruction.clear();
	}

	void Scene::BuildSnapshotRecursive()
	{
		frameSnapshot.clear();

		TraverseRoots(false, [&](uint32_t index, GameObject&)
			{
				frameSnapshot.push_back(index);

				return true; // continue traversal
			});
	}

	void Scene::Exit()
	{
		for (auto& entry : entries)
		{
			entry.state = ObjectEntryState::DEAD;
			entry.generation++;
			entry.object.reset();
		}

		entries.clear();
		freeSlots.clear();
		gameObjectRoots.clear();
		gameObjectsToInitialize.clear();
	}

	void Scene::Init()
	{
		std::vector<ObjectHandle<GameObject>> initialized;
		initialized.reserve(gameObjectsToInitialize.size());

		while (!gameObjectsToInitialize.empty())
		{
			auto batch = std::move(gameObjectsToInitialize);

			gameObjectsToInitialize.clear();

			for (const auto& index : batch)
			{
				auto& entry = entries[index];
				auto* obj = entry.object.get();

				if (!obj)
					continue;

				obj->componentBucket->ForEach([](auto& comp)
				{
					comp.Init();
				});

				initialized.push_back(obj->GetObjectHandle());
			}
		}

		isRunning = true;

		for (const auto& obj : initialized)
		{
			if (!obj) 
				continue;

			obj->componentBucket->ForEach([](auto& comp)
			{
				comp.Start();
			});
		}
	}

	void Scene::Update(float deltaTime)
	{
		// The order here matters!
		ProcessPendingDestructions();
		BuildSnapshotRecursive();

		for (const auto& entryIndex : frameSnapshot)
		{
			auto& entry = entries[entryIndex];
			auto* obj = entry.object.get();

			if (obj->componentBucket == nullptr)
				continue;

			obj->componentBucket->ForEach([&entry, entryIndex, deltaTime, this](auto& comp)
			{
				if (!IsValid(entryIndex, entry.generation))
					return;

				if (!comp.IsActive())
					return;

				comp.Update(deltaTime);
			});
		}
	}

	void Scene::LateUpdate(float deltaTime)
	{
		for (const auto& entryIndex : frameSnapshot)
		{
			auto& entry = entries[entryIndex];
			auto* obj = entry.object.get();

			if (obj->componentBucket == nullptr)
				continue;

			obj->componentBucket->ForEach([&entry, entryIndex, deltaTime, this](auto& comp)
			{
				if (!IsValid(entryIndex, entry.generation))
					return;

				if (!comp.IsActive())
					return;

				comp.LateUpdate(deltaTime);
			});
		}
	}

	void Scene::Draw()
	{
		for (const auto& entryIndex : frameSnapshot)
		{
			auto& entry = entries[entryIndex];
			auto* obj = entry.object.get();

			if (obj->componentBucket == nullptr)
				continue;

			obj->componentBucket->ForEach([&entry, entryIndex, this](auto& comp)
			{
				if (!IsValid(entryIndex, entry.generation))
					return;

				if (!comp.IsActive())
					return;

				comp.Draw();
			});
		}
	}
}