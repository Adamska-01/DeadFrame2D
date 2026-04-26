#pragma once
#include "Data/Entity/ObjectEntry.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Scene/Abstractions/ISceneHandleProvider.h"
#include <type_traits>
#include <vector>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class ComponentBucket;
	class DispatchableEvent;
	class GameComponent;


	class DF2D_API Scene : public std::enable_shared_from_this<Scene>, public ISceneHandleProvider
	{
		friend class GameObject;


	private:
		bool isRunning;

		std::vector<uint32_t> gameObjectsToInitialize;

		std::vector<uint32_t> objectsPendingDestruction;

		std::vector<uint32_t> freeSlots;

		std::vector<uint32_t> gameObjectRoots;

		std::vector<uint32_t> frameSnapshot;

		std::vector<DeadFrame2D::Data::ObjectEntry> entries;


		uint32_t FindFreeSlot();


		GameObject* GetAt(uint32_t index) const override;

		bool IsValid(uint32_t index, uint32_t generation) const override;


		template<typename T, typename... Args>
		ObjectHandle<T> Instantiate(Args&&... args);

		template<typename Fn>
		void TraverseRoots(bool includeInactive, Fn&& fn);


		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameObjectHierarchyChangeHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameComponentAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


		void SendGameObjectCreatedEvent(const ObjectHandle<GameObject>& obj);

		void ProcessPendingDestructions();

		void BuildSnapshotRecursive();

		void Exit();


	public:
		Scene();

		virtual ~Scene();

		Scene(const Scene&) = delete;

		Scene& operator=(const Scene&) = delete;

		Scene(Scene&&) noexcept = default;

		Scene& operator=(Scene&&) noexcept = default;


		virtual void Enter() = 0;


		void Init();


		virtual void Update(float deltaTime);

		virtual void LateUpdate(float deltaTime);

		virtual void Draw();


		template <typename T>
		ComponentHandle<T> FindObjectOfType(bool includeInactive = false);

		template <typename T>
		std::vector<ComponentHandle<T>> FindObjectsOfType(bool includeInactive = false);
	};
}


#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Component/GameComponent.h"


namespace DeadFrame2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> Scene::Instantiate(Args && ...args)
	{
		using namespace DeadFrame2D::Data;


		auto index = FindFreeSlot();

		auto& entry = entries[index];
		entry.object = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		entry.state = ObjectEntryState::ALIVE;

		auto handle = ObjectHandle<T>(shared_from_this(), index, entry.generation);

		entry.object->SetThisHandle(handle);

		// Every GameObject starts with a Transform, which needs to be linked to the owner post-creation.
		entry.object->componentBucket->ForEach([&handle, obj = entry.object.get()](GameComponent& comp)
			{
				obj->componentBucket->LinkComponentToOwner(handle, &comp);
			});

		handle->ConstructGameObject();

		gameObjectRoots.push_back(index);

		if (!isRunning)
		{
			gameObjectsToInitialize.push_back(index);
		}

		SendGameObjectCreatedEvent(handle);

		return handle;
	}

	template<typename Fn>
	inline void Scene::TraverseRoots(bool includeInactive, Fn&& fn)
	{
		std::vector<uint32_t> stack;
		stack.reserve(gameObjectRoots.size());

		for (auto root : gameObjectRoots)
		{
			stack.push_back(root);
		}

		while (!stack.empty())
		{
			auto index = stack.back();

			stack.pop_back();

			const auto& entry = entries[index];
			auto* obj = entry.object.get();

			if (!obj)
				continue;

			if (!includeInactive && !obj->IsActive())
				continue;

			if (!IsValid(index, entry.generation))
				continue;

			if (!fn(index, *obj))
				return;

			const auto& children = obj->GetChildren();
			for (auto it = children.rbegin(); it != children.rend(); ++it)
			{
				if (*it)
				{
					stack.push_back((*it).GetIndex());
				}
			}
		}
	}


	template<typename T>
	inline ComponentHandle<T> Scene::FindObjectOfType(bool includeInactive)
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		ComponentHandle<T> result;

		TraverseRoots(includeInactive, [&](uint32_t, GameObject& obj)
		{
			auto component = obj.GetComponent<T>();

			if (component)
			{
				result = component;

				return false; // stop traversal
			}

			return true; // keep going
		});

		return result;
	}

	template <typename T>
	inline std::vector<ComponentHandle<T>> Scene::FindObjectsOfType(bool includeInactive)
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		std::vector<ComponentHandle<T>> results;

		TraverseScene(includeInactive, [&](uint32_t, GameObject& obj)
		{
			auto component = obj.GetComponent<T>();

			if (component)
			{
				results.push_back(component);
			}

			return true; // keep traversing
		});

		return results;
	}
}