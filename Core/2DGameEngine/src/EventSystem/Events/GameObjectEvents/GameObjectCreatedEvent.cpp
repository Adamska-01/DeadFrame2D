#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"


GameObjectCreatedEvent::GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectCreated)
	: gameObjectCreated(gameObjectCreated)
{
}
