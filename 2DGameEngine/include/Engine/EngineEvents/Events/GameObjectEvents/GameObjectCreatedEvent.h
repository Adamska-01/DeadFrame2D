#pragma once
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	class GameObjectCreatedEvent : public DispatchableEvent
	{
	public:
		std::shared_ptr<GameObject> gameObjectCreated;


		GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectDestroyed);
	};
}