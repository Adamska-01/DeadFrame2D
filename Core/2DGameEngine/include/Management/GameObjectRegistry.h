#pragma once
#include "Components/Collisions/Collider2D.h"
#include "Tools/Collisions/CollisionHandler.h"
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"
#include <memory>
#include <vector>


class GameObjectRegistry
{
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	std::vector<Collider2D*> colliders;

	CollisionHandler collisionHandler;


	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	GameObjectRegistry();

	~GameObjectRegistry();


	void Update(float deltaTime);

	void Draw();
};