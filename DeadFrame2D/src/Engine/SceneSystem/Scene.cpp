#include "Engine/Components/UI/Canvas.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/SceneSystem/Scene.h"
#include <algorithm>
#include <cassert>


namespace DeadFrame2D::Engine
{
	Scene::Scene()
	{
		isRunning = false;

		Exit();

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this, &Scene::GameObjectDestroyedHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(ChildGameObjectAddedEvent)), this, &Scene::ChildGameObjectAddedHandler);
	}

	Scene::~Scene()
	{
		Exit();

		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(ChildGameObjectAddedEvent)), this);
	}

	uint32_t Scene::FindFreeSlot()
	{
		for (size_t i = 0; i < entries.size(); ++i)
		{
			if (!entries[i].alive)
				return static_cast<uint32_t>(i);
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
		
		return e.alive && e.generation == generation;
	}

	void Scene::SendGameObjectCreatedEvent(const ObjectHandle<GameObject>& obj)
	{
		EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(obj));
	}

	void Scene::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

		if (!gameObjEvent || gameObjEvent->gameObjectDestroyed == nullptr)
			return;

		auto toDestroyPtr = gameObjEvent->gameObjectDestroyed;

		// Mark for destruction
		objectsPendingDestroy.push_back(toDestroyPtr);
	}

	void Scene::ChildGameObjectAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameObjEvent = DispatchableEvent::SafeCast<ChildGameObjectAddedEvent>(dispatchableEvent);

		if (!gameObjEvent || gameObjEvent->childGameObject == nullptr)
			return;

		childAddedPendingAction.push_back(gameObjEvent->childGameObject);
	}

	void Scene::CleanupDestroyedObjects()
	{
		if (objectsPendingDestroy.empty())
			return;

		for (const auto& target : objectsPendingDestroy)
		{
			if (target == nullptr)
				continue;

			const auto matchesTarget = [&target](const ObjectHandle<GameObject>& obj)
				{
					return obj == target;
				};

			gameObjects.erase(
				std::remove_if(
					gameObjects.begin(), 
					gameObjects.end(), 
					matchesTarget),
				gameObjects.end());

			if (target->GetParent() != nullptr)
				continue;

			gameObjectParents.erase(
				std::remove_if(
					gameObjectParents.begin(), 
					gameObjectParents.end(), 
					matchesTarget),
				gameObjectParents.end());
		}

		for (const auto& target : objectsPendingDestroy)
		{
			auto& entry = entries[target.index];

			entry.alive = false;
			entry.generation++;
			entry.object.reset();
		}

		objectsPendingDestroy.clear();
	}

	void Scene::Exit()
	{
		for (auto& entry : entries)
		{
			entry.alive = false;
			entry.generation++;
			entry.object.reset();
		}

		entries.clear();
		gameObjects.clear();
		gameObjectParents.clear();
		gameObjectsToInitialize.clear();
		objectsPendingCreation.clear();
		objectsPendingDestroy.clear();
	}

	void Scene::Init()
	{
		// TODO: The way Init and Start are called feels wrong. Fix it (All the Inits must be called before all the Starts, and must follow instantiation-order)
		// Some game objects might create other game objects in their Init function.
		// However, we want to ensure that only the game objects in the current
		// initialization queue are considered part of this initial phase. 
		// All the game objects created after this step are initalised at creation-time.
		std::vector<ObjectHandle<GameObject>> initialized;
		initialized.reserve(gameObjectsToInitialize.size());

		while (!gameObjectsToInitialize.empty())
		{
			auto batch = std::move(gameObjectsToInitialize);
			gameObjectsToInitialize.clear();

			for (const auto& obj : batch)
			{
				if (!obj)
					continue;

				obj->Init();

				initialized.push_back(obj);
			}
		}

		isRunning = true;

		for (const auto& obj : initialized)
		{
			if (!obj) 
				continue;

			obj->Start();
		}
	}

	void Scene::Update(float deltaTime)
	{
		auto creationPendingSize = objectsPendingCreation.size();

		if (creationPendingSize > 0)
		{
			for (size_t i = 0; i < creationPendingSize; ++i)
			{
				auto& obj = objectsPendingCreation[i];

				gameObjectParents.push_back(obj);
			}

			objectsPendingCreation.clear();
		}
	
		auto childAddedPendingActionSize = childAddedPendingAction.size();

		if (childAddedPendingActionSize > 0)
		{
			for (size_t i = 0; i < childAddedPendingActionSize; ++i)
			{
				const auto& childObj = childAddedPendingAction[i];

				gameObjectParents.erase(
					std::remove_if(
						gameObjectParents.begin(),
						gameObjectParents.end(),
						[&childObj](const auto& obj)
						{
							return obj == childObj;
						}),
					gameObjectParents.end());
			}

			childAddedPendingAction.clear();
		}

		auto parentsSize = gameObjectParents.size();

		for (size_t i = 0; i < parentsSize; ++i)
		{
			auto& obj = gameObjectParents[i];
		
			if (!obj->IsActive())
				continue;

			obj->Update(deltaTime);
		}
	}

	void Scene::LateUpdate(float deltaTime)
	{
		auto parentsSize = gameObjectParents.size();

		for (size_t i = 0; i < parentsSize; ++i)
		{
			auto& obj = gameObjectParents[i];
		
			if (!obj->IsActive())
				continue;

			obj->LateUpdate(deltaTime);
		}

		CleanupDestroyedObjects();
	}

	void Scene::Draw()
	{
		for (const auto& obj : gameObjectParents)
		{
			if (!obj->IsActive())
				continue;

			obj->Draw();
		}
	}
}