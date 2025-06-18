#include "EventSystem/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"


ChildGameObjectAddedEvent::ChildGameObjectAddedEvent(std::weak_ptr<GameObject> childGameObject)
	: childGameObject(childGameObject)
{
}