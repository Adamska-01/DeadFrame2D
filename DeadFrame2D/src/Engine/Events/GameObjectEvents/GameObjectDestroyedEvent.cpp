#include "Engine/Entity/GameObject.h"
#include "Engine/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	GameObjectDestroyedEvent::GameObjectDestroyedEvent(ObjectHandle<GameObject> gameObjectDestroyed)
		: gameObjectDestroyed(gameObjectDestroyed)
	{
	}
}