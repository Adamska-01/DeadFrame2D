#pragma once
#include "DF2D_API.h"
#include "DispatchableEvent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <memory>
#include <typeindex>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	using EventHandler = DeadFrame2D::Utilities::MulticastDelegate<std::shared_ptr<DispatchableEvent>>;


	class DF2D_API EventDispatcher
	{
	private:
		static std::unordered_map<std::type_index, EventHandler> eventHandlers;


		EventDispatcher() = delete;


	public:
		template<typename T>
		static void RegisterEventHandler(const std::type_index& eventType, T* instance, void(T::* func)(std::shared_ptr<DispatchableEvent>));

		template<typename T>
		static void DeregisterEventHandler(const std::type_index& eventType, T* instance);


		static void SendEvent(std::shared_ptr<DispatchableEvent> event);
	};
	
}


namespace DeadFrame2D::Engine
{
	template<typename T>
	inline void EventDispatcher::RegisterEventHandler(const std::type_index& eventType, T* instance, void(T::* func)(std::shared_ptr<DispatchableEvent>))
	{
		if (eventHandlers.find(eventType) == eventHandlers.end())
		{
			eventHandlers[eventType] = EventHandler();
		}

		eventHandlers[eventType].AddRaw(instance, func);
	}

	template<typename T>
	inline void EventDispatcher::DeregisterEventHandler(const std::type_index& eventType, T* instance)
	{
		auto it = eventHandlers.find(eventType);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << eventType.name() << std::endl;
			return;
		}

		it->second.RemoveByListener(instance);

		if (it->second.IsEmpty())
			eventHandlers.erase(it);
	}
}