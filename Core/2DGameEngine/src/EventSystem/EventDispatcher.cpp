#include "EventSystem/EventDispatcher.h"
#include <iostream>


std::unordered_map<std::type_index, EventHandler> EventDispatcher::eventHandlers;


void EventDispatcher::RegisterEventHandler(const std::type_index& eventType, const std::function<void(std::shared_ptr<DispatchableEvent>)>& handler)
{
	if (eventHandlers.find(eventType) == eventHandlers.end())
	{
		eventHandlers[eventType] = EventHandler();
	}
	
	eventHandlers[eventType] += handler;
}

void EventDispatcher::DeregisterEventHandler(const std::type_index& eventType, const std::function<void(std::shared_ptr<DispatchableEvent>)>& handler)
{
	if (eventHandlers.find(eventType) == eventHandlers.end())
	{
		std::cout << "No handler registered for event type: " << eventType.name() << std::endl;

		return;
	}
	
	eventHandlers[eventType] -= handler;
	
	if (eventHandlers[eventType].IsEmpty())
	{
		eventHandlers.erase(eventType);
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