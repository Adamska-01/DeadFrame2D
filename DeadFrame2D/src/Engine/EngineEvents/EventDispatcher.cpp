#include "Engine/EngineEvents/EventDispatcher.h"
#include <iostream>


namespace DeadFrame2D::Engine
{
	std::unordered_map<std::type_index, EventHandler> EventDispatcher::eventHandlers;


	void EventDispatcher::SendEvent(std::shared_ptr<DispatchableEvent> event)
	{
		auto type = std::type_index(typeid(*event));
	
		if (eventHandlers.find(type) == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << type.name() << std::endl;
		
			return;
		}

		eventHandlers[type].Broadcast(event);
	}
}