#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


GameObjectDestroyedEvent::GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed)
	: gameObjectDestroyed(gameObjectDestroyed)
{
}