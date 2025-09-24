#include "Engine/EngineEvents/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"


namespace DeadFrame2D::Engine
{
	ChildGameObjectAddedEvent::ChildGameObjectAddedEvent(std::weak_ptr<GameObject> childGameObject)
		: childGameObject(childGameObject)
	{
	}
}