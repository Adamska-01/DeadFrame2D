#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API GameObjectCreatedEvent : public DispatchableEvent
	{
	private:
		ObjectHandle<GameObject> gameObjectCreated;


	public:
		GameObjectCreatedEvent(const ObjectHandle<GameObject>& createdObject);


		const ObjectHandle<GameObject>& GetGameObject() const;
	};
}