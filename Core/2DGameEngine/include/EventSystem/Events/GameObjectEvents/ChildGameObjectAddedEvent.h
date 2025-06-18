#pragma once
#include "EventSystem/DispatchableEvent.h"
#include <memory>


class GameObject;


class ChildGameObjectAddedEvent : public DispatchableEvent
{
public:
	std::weak_ptr<GameObject> childGameObject;


	ChildGameObjectAddedEvent(std::weak_ptr<GameObject> childGameObject);
};