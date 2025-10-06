#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API ChildGameObjectAddedEvent : public DispatchableEvent
	{
	public:
		std::weak_ptr<GameObject> childGameObject;


		ChildGameObjectAddedEvent(std::weak_ptr<GameObject> childGameObject);
	};
}