#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	GameObjectDestroyedEvent::GameObjectDestroyedEvent(ObjectHandle<GameObject> gameObjectDestroyed)
		: gameObjectDestroyed(gameObjectDestroyed)
	{
	}
}