#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API GameObjectCreatedEvent : public DispatchableEvent
	{
	public:
		std::shared_ptr<GameObject> gameObjectCreated;


		GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectDestroyed);
	};
}