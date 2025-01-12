#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"


class GameObjectCreatedEvent : public DispatchableEvent
{
public:
	GameObject* gameObjectCreated;


	GameObjectCreatedEvent(GameObject* gameObjectDestroyed);
};