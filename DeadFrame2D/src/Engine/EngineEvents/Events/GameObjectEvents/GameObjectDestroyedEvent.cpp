#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	GameObjectDestroyedEvent::GameObjectDestroyedEvent(std::weak_ptr<GameObject> gameObjectDestroyed)
		: gameObjectDestroyed(gameObjectDestroyed)
	{
	}
}