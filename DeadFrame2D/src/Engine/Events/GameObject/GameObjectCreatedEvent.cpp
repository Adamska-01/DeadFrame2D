#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/Events/GameObject/GameObjectCreatedEvent.h"


namespace DF2D::Engine
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