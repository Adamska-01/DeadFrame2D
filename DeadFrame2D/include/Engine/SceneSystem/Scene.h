#pragma once
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObject.h"
#include <type_traits>
#include <vector>


namespace DeadFrame2D::Engine
{
	class DF2D_API Scene
	{
	private:
		bool isRunning;

		std::vector<std::weak_ptr<GameObject>> gameObjectsToInitialize;

		std::vector<std::shared_ptr<GameObject>> objectsPendingCreation;

		std::vector<std::weak_ptr<GameObject>> childAddedPendingAction;

		std::vector<std::weak_ptr<GameObject>> objectsPendingDestroy;


	protected:
		std::vector<std::shared_ptr<GameObject>> gameObjects;

		std::vector<std::shared_ptr<GameObject>> gameObjectParents;


		void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void ChildGameObjectAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


		void CleanupDestroyedObjects();


	public:
		Scene();

		virtual ~Scene();


		virtual void Enter() = 0;


		void Init();


		virtual void Update(float deltaTime);

		virtual void LateUpdate(float deltaTime);

		virtual void Draw();


		void Exit();


		template <typename T>
		ComponentHandle<T> FindObjectOfType();

		template <typename T>
		std::vector<ComponentHandle<T>> FindObjectsOfType();
	};
}


namespace DeadFrame2D::Engine
{
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
			if (component != nullptr)
			{
				results.push_back(component);
			}
		}

		return results;
	}
}