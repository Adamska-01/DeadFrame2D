#pragma once
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/GameObject.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObjectDestroyedEvent : public DispatchableEvent
	{
	public:
		std::weak_ptr<GameObject> gameObjectDestroyed;


		GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed);
	};
}