#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API GameObjectDestroyedEvent : public DispatchableEvent
	{
	public:
		ObjectHandle<GameObject> gameObjectDestroyed;


		GameObjectDestroyedEvent(ObjectHandle<GameObject> gameObjectDestroyed);
	};
}