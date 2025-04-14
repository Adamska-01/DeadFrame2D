#pragma once
#include "Components/GameComponent.h"
#include "GameObject.h"
#include "Tools/Collisions/CollisionHandler.h"
#include <type_traits>
#include <vector>


class GameScene
{
protected:
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	std::vector<Collider2D*> colliders;

	CollisionHandler collisionHandler;


	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	GameScene();

	virtual ~GameScene();


	virtual void Enter() = 0;
	
	virtual void Exit() = 0;


	virtual void Update(float deltaTime);

	virtual void Draw();
	

	template <typename T>
	T* FindObjectOfType();
};


template<typename T>
inline T* GameScene::FindObjectOfType()
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