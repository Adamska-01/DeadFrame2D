#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API ChildGameObjectAddedEvent : public DispatchableEvent
	{
	public:
		ObjectHandle<GameObject> childGameObject;


		ChildGameObjectAddedEvent(ObjectHandle<GameObject> childGameObject);
	};
}