#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/Entity/GameObject.h"


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