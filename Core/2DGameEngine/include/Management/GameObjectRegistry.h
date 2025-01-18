#pragma once
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"
#include <memory>
#include <vector>


class GameObjectRegistry
{
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;


	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	GameObjectRegistry();

	~GameObjectRegistry();


	void Update(float deltaTime);

	void Draw();
};