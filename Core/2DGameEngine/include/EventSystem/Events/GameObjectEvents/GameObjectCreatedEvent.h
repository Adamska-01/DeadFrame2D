#pragma once
#include "EventSystem/DispatchableEvent.h"


class GameObject;


class GameObjectCreatedEvent : public DispatchableEvent
{
public:
	std::shared_ptr<GameObject> gameObjectCreated;


	GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectDestroyed);
};