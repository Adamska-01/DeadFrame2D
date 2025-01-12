#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"


class GameObjectDestroyedEvent : public DispatchableEvent
{
public:
	const GameObject* gameObjectDestroyed;


	GameObjectDestroyedEvent(const GameObject* gameObjectDestroyed);
};