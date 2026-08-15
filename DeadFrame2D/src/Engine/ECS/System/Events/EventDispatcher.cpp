#include "Engine/ECS/System/Events/EventDispatcher.h"
#include <iostream>


namespace DF2D::Engine
{
	void EventDispatcher::SendEvent(std::shared_ptr<DispatchableEvent> event)
	{
		auto type = std::type_index(typeid(*event));

		auto it = eventHandlers.find(type);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << type.name() << std::endl;

			return;
		}

		it->second.Broadcast(event);
	}
}