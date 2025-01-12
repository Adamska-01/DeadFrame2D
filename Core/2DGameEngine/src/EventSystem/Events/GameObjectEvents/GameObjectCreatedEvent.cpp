#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"


GameObjectCreatedEvent::GameObjectCreatedEvent(GameObject* gameObjectCreated)
	: gameObjectCreated(gameObjectCreated)
{
}
