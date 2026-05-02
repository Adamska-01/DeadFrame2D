#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DF2D::Engine
{
	class GameObject;


	class DF2D_API GameObjectDestroyedEvent : public DispatchableEvent
	{
	public:
		ObjectHandle<GameObject> gameObjectDestroyed;


		GameObjectDestroyedEvent(ObjectHandle<GameObject> gameObjectDestroyed);
	};
}