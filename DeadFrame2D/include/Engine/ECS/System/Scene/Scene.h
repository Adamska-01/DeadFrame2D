#pragma once
#include "Data/Services/Scene/ObjectEntry.h"
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObjectConstructionContext.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Scene/Abstractions/ISceneHandleProvider.h"
#include <type_traits>
#include <vector>


namespace DF2D::Engine
{
	class GameObject;
	class ComponentBucket;
	class DispatchableEvent;
	class EventDispatcher;
	class GameComponent;


	class DF2D_API Scene : public std::enable_shared_from_this<Scene>, public ISceneHandleProvider
	{
		friend class SceneManager;


	private:
		bool isRunning;

		EventDispatcher& eventDispatcher;

		std::vector<uint32_t> gameObjectsToInitialize;

		std::vector<uint32_t> objectsPendingDestruction;

		std::vector<uint32_t> freeSlots;

		std::vector<uint32_t> gameObjectRoots;

		std::vector<uint32_t> frameSnapshot;

		std::vector<Data::ObjectEntry> entries;


		uint32_t FindFreeSlot();


		GameObject* GetAt(uint32_t index) const override;

		bool IsValid(uint32_t index, uint32_t generation) const override;


		template<typename Fn>
		void TraverseRoots(bool includeInactive, Fn&& fn);


		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameObjectHierarchyChangeHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameComponentAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


		void SendGameObjectCreatedEvent(const ObjectHandle<GameObject>& obj);

		void ProcessPendingDestructions();

		void BuildSnapshotRecursive();

		void Exit();


	protected:
		template<typename T, typename... Args>
		ObjectHandle<T> Instantiate(Data::CoreContext coreCtx, Data::ServiceContext serviceCtx, Args&&... args);


	public:
		// eventDispatcher: nullptr in production (resolved from SceneManager::GetServiceContext()).
		// Tests pass an explicit instance to isolate a Scene from SceneManager's shared state entirely.
		explicit Scene(EventDispatcher* eventDispatcher = nullptr);

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
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"


namespace DF2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> Scene::Instantiate(Data::CoreContext coreCtx, Data::ServiceContext serviceCtx, Args && ...args)
	{
		using namespace DF2D::Data;


		auto index = FindFreeSlot();

		auto& entry = entries[index];
		entry.state = ObjectEntryState::ALIVE;

		// Reserve raw storage and register it before construction, so a T under
		// construction can resolve its own handle (see GameObjectConstructionContext)
		// and e.g. parent freshly spawned children to itself from its own constructor.
		void* storage = ::operator new(sizeof(T));
		entry.object = std::unique_ptr<GameObject>(static_cast<GameObject*>(static_cast<T*>(storage)));

		auto handle = ObjectHandle<T>(shared_from_this(), index, entry.generation);

		// Every object spawned by this Scene carries the SAME dispatcher this Scene
		// registered its handlers against, regardless of where it was resolved from.
		serviceCtx.eventDispatcher = &eventDispatcher;

		GameObjectConstructionContext constructionContext(handle, coreCtx, serviceCtx);

		try
		{
			new (storage) T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			// The already-constructed base subobjects were destroyed by the language
			// as part of unwinding out of the placement-new expression above; only
			// release the raw storage, don't run GameObject's destructor a second time.
			entry.object.release();
			::operator delete(storage);

			entry.state = ObjectEntryState::DEAD;
			freeSlots.push_back(index);

			throw;
		}

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

		TraverseRoots(includeInactive, [&](uint32_t, GameObject& obj)
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