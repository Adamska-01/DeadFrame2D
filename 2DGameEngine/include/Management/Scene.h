#pragma once
#include "Components/GameComponent.h"
#include "GameObject.h"
#include <type_traits>
#include <vector>


class Scene
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
	T* FindObjectOfType();
	
	template <typename T>
	std::vector<T*> FindObjectsOfType();
};


template<typename T>
inline T* Scene::FindObjectOfType()
{
	static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

	for (const auto& object : gameObjects)
	{
		auto component = object->GetComponent<T>();

		if (component == nullptr)
			continue;

		return component;
	}

	return nullptr;
}

template <typename T>
inline std::vector<T*> Scene::FindObjectsOfType()
{
	static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

	std::vector<T*> results;

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