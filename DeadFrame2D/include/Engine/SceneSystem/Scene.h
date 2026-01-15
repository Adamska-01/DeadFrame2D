#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/Handles/GameObject/ObjectEntry.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"
#include "Engine/SceneSystem/ISceneHandleProvider.h"
#include <type_traits>
#include <vector>


namespace DeadFrame2D::Engine
{
	class GameObject;

	class DispatchableEvent;


	class DF2D_API Scene : public std::enable_shared_from_this<Scene>, public ISceneHandleProvider
	{
		friend class ObjectHandleBase;

		friend class GameObject;


	private:
		bool isRunning;

		std::vector<ObjectHandle<GameObject>> gameObjectsToInitialize;

		std::vector<ObjectHandle<GameObject>> objectsPendingCreation;

		std::vector<ObjectHandle<GameObject>> childAddedPendingAction;

		std::vector<ObjectHandle<GameObject>> objectsPendingDestroy;


		uint32_t FindFreeSlot();

		GameObject* GetAt(uint32_t index) const override;

		bool IsValid(uint32_t index, uint32_t generation) const override;


		void SendGameObjectCreatedEvent(const ObjectHandle<GameObject>& obj);


		template<typename T, typename... Args>
		ObjectHandle<T> Instantiate(Args&&... args);


	protected:
		std::vector<ObjectEntry> entries;

		std::vector<ObjectHandle<GameObject>> gameObjects;

		std::vector<ObjectHandle<GameObject>> gameObjectRoots;


		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void ChildGameObjectAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


		void CleanupDestroyedObjects();

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
		ComponentHandle<T> FindObjectOfType();

		template <typename T>
		std::vector<ComponentHandle<T>> FindObjectsOfType();
	};
}


#include "Engine/Entity/GameObject.h"
#include "Engine/Components/GameComponent.h"


namespace DeadFrame2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> Scene::Instantiate(Args && ...args)
	{
		auto index = FindFreeSlot();

		auto& entry = entries[index];
		entry.object = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		entry.alive = true;

		auto handle = ObjectHandle<T>(shared_from_this(), index, entry.generation);

		entry.object->SetThisHandle(handle);

		// Every GameObject starts with a Transform, which needs to be linked to the owner post-creation.
		entry.object->componentBucket->ForEach([&handle, obj = entry.object.get()](GameComponent& comp)
			{
				obj->componentBucket->LinkComponentToOwner(handle, &comp);
			});

		handle->ConstructGameObject();

		gameObjects.push_back(handle);
		objectsPendingCreation.push_back(handle);

		if (isRunning)
		{
			handle->Init();
			handle->Start();
		}

		gameObjectsToInitialize.push_back(handle);

		SendGameObjectCreatedEvent(handle);

		return handle;
	}


	template<typename T>
	inline ComponentHandle<T> Scene::FindObjectOfType()
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		for (const auto& object : gameObjects)
		{
			auto component = object->GetComponent<T>();

			if (component == nullptr)
				continue;

			return component;
		}

		return ComponentHandle<T>();
	}

	template <typename T>
	inline std::vector<ComponentHandle<T>> Scene::FindObjectsOfType()
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		std::vector<ComponentHandle<T>> results;

		for (const auto& object : gameObjects)
		{
			auto component = object->GetComponent<T>();
			
			if (component == nullptr)
				continue;

			results.push_back(component);
		}

		return results;
	}
}