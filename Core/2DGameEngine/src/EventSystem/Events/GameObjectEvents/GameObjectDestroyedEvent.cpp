#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


GameObjectDestroyedEvent::GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed)
	: gameObjectDestroyed(gameObjectDestroyed)
{
}