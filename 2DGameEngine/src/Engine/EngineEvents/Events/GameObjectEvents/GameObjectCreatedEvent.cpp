#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"


namespace DeadFrame2D::Engine
{
	GameObjectCreatedEvent::GameObjectCreatedEvent(std::shared_ptr<GameObject> gameObjectCreated)
		: gameObjectCreated(gameObjectCreated)
	{
	}
}