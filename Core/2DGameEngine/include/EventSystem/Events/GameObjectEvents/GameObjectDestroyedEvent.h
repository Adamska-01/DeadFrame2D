#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "GameObject.h"
#include <memory>


class GameObjectDestroyedEvent : public DispatchableEvent
{
public:
	std::weak_ptr<GameObject> gameObjectDestroyed;


	GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed);
};