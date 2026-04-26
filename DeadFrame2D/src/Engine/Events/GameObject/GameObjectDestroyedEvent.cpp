#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/Events/GameObject/GameObjectDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	GameObjectDestroyedEvent::GameObjectDestroyedEvent(ObjectHandle<GameObject> gameObjectDestroyed)
		: gameObjectDestroyed(gameObjectDestroyed)
	{
	}
}