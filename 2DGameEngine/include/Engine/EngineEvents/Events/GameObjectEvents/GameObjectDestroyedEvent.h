#pragma once
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/GameObject.h"
#include <memory>


class GameObjectDestroyedEvent : public DispatchableEvent
{
public:
	std::weak_ptr<GameObject> gameObjectDestroyed;


	GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed);
};