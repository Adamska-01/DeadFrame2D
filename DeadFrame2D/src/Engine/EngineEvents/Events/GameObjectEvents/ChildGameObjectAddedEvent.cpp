#include "Engine/EngineEvents/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	ChildGameObjectAddedEvent::ChildGameObjectAddedEvent(ObjectHandle<GameObject> childGameObject)
		: childGameObject(childGameObject)
	{
	}
}