#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/GameObject.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class DF2D_API GameObjectDestroyedEvent : public DispatchableEvent
	{
	public:
		std::weak_ptr<GameObject> gameObjectDestroyed;


		GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed);
	};
}