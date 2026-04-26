#include "Engine/Entity/GameObject.h"
#include "Engine/Events/GameObjectEvents/GameObjectCreatedEvent.h"


namespace DeadFrame2D::Engine
{
	GameObjectCreatedEvent::GameObjectCreatedEvent(const ObjectHandle<GameObject>& createdObject)
		: gameObjectCreated(createdObject)
	{
	}

	const ObjectHandle<GameObject>& GameObjectCreatedEvent::GetGameObject() const
	{
		return gameObjectCreated;
	}
}