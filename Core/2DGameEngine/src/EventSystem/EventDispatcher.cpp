#include "EventSystem/EventDispatcher.h"
#include <iostream>


void EventDispatcher::RegisterEventHandler(const std::type_index& eventType, const EventHandler& handler)
{
	if (eventHandlers.find(eventType) == eventHandlers.end())
	{
		eventHandlers[eventType] = handler;
	}
	else
	{
		eventHandlers[eventType] += handler;
	}
}

void EventDispatcher::DeregisterEventHandler(const std::type_index& eventType, const EventHandler& handler)
{
	auto it = eventHandlers.find(eventType);

	if (it == eventHandlers.end())
		return;

	it->second -= handler;

	if (it->second.IsEmpty())
	{
		eventHandlers.erase(it);
	}
}

void EventDispatcher::SendEvent(std::shared_ptr<DispatchableEvent> event)
{
	auto type = std::type_index(typeid(*event));
	
	if (eventHandlers.find(type) == eventHandlers.end())
	{
		std::cout << "No handler registered for event type: " << type.name() << std::endl;
		
		return;
	}

	eventHandlers[type](event);
}