#pragma once
#include "Components/Collisions/Collider2D.h"
#include "EventSystem/DispatchableEvent.h"
#include "Tools/Collisions/CollisionHandler.h"
#include <memory>
#include <vector>


class GameObject;


class GameObjectRegistry
{
private:
	static std::vector<std::shared_ptr<GameObject>> gameObjects;

	std::vector<Collider2D*> colliders;

	CollisionHandler collisionHandler;


	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	GameObjectRegistry();

	~GameObjectRegistry();


	void Update(float deltaTime);

	void Draw();


	template <typename T>
	static T* FindObjectOfType();
};


template<typename T>
inline T* GameObjectRegistry::FindObjectOfType()
{
	static_assert(std::is_base_of<GameComponent, T>::value, "T must derive from GameComponent");

	for (const auto& object : gameObjects)
	{
		auto component = object->GetComponent<T>();

		if (component == nullptr)
			continue;

		return component;
	}
}