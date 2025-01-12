#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


GameObjectDestroyedEvent::GameObjectDestroyedEvent(const GameObject* gameObjectDestroyed)
	: gameObjectDestroyed(gameObjectDestroyed)
{
}