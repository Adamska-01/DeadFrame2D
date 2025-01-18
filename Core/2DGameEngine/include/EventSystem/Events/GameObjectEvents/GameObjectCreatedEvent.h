#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"


class GameObjectCreatedEvent : public DispatchableEvent
{
public:
	std::shared_ptr<GameObject> gameObjectCreated;


	GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectDestroyed);
};