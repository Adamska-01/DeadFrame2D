#pragma once
#include "DF2D_API.h"
#include "DispatchableEvent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>


namespace DF2D::Engine
{
	class ComponentHandleBase;
	class ObjectHandleBase;


	using EventHandler = Utilities::MulticastDelegate<std::shared_ptr<DispatchableEvent>>;


	class DF2D_API EventDispatcher
	{
	private:
		std::unordered_map<std::type_index, EventHandler> eventHandlers;


	public:
		EventDispatcher() = default;

		~EventDispatcher() = default;

		EventDispatcher(const EventDispatcher&) = delete;

		EventDispatcher& operator=(const EventDispatcher&) = delete;


		template<typename EventT, typename T>
		void RegisterEventHandler(T* instance, void(T::* func)(std::shared_ptr<DispatchableEvent>));

		template<typename EventT>
		void RegisterEventHandler(const ComponentHandleBase& handle, std::function<void(std::shared_ptr<DispatchableEvent>)> handler);

		template<typename EventT>
		void RegisterEventHandler(const ObjectHandleBase& handle, std::function<void(std::shared_ptr<DispatchableEvent>)> handler);

		template<typename EventT, typename T>
		void RegisterEventHandler(const std::shared_ptr<T>& instance, void(T::* func)(std::shared_ptr<DispatchableEvent>));


		template<typename EventT, typename T>
		void DeregisterEventHandler(T* instance);

		template<typename EventT>
		void DeregisterEventHandler(const ComponentHandleBase& handle);

		template<typename EventT>
		void DeregisterEventHandler(const ObjectHandleBase& handle);

		template<typename EventT, typename T>
		void DeregisterEventHandler(const std::shared_ptr<T>& instance);


		void SendEvent(std::shared_ptr<DispatchableEvent> event);
	};

}


namespace DF2D::Engine
{
	template<typename EventT, typename T>
	inline void EventDispatcher::RegisterEventHandler(T* instance, void(T::* func)(std::shared_ptr<DispatchableEvent>))
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		eventHandlers[std::type_index(typeid(EventT))].AddRaw(instance, func);
	}

	template<typename EventT>
	inline void EventDispatcher::RegisterEventHandler(const ComponentHandleBase& handle, std::function<void(std::shared_ptr<DispatchableEvent>)> handler)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		eventHandlers[std::type_index(typeid(EventT))].AddHandle(handle, std::move(handler));
	}

	template<typename EventT>
	inline void EventDispatcher::RegisterEventHandler(const ObjectHandleBase& handle, std::function<void(std::shared_ptr<DispatchableEvent>)> handler)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		eventHandlers[std::type_index(typeid(EventT))].AddHandle(handle, std::move(handler));
	}

	template<typename EventT, typename T>
	inline void EventDispatcher::RegisterEventHandler(const std::shared_ptr<T>& instance, void(T::* func)(std::shared_ptr<DispatchableEvent>))
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		eventHandlers[std::type_index(typeid(EventT))].AddShared(instance, func);
	}

	template<typename EventT, typename T>
	inline void EventDispatcher::DeregisterEventHandler(T* instance)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		auto eventType = std::type_index(typeid(EventT));

		auto it = eventHandlers.find(eventType);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << eventType.name() << std::endl;
			return;
		}

		it->second.RemoveByListener(instance);

		if (it->second.IsEmpty())
		{
			eventHandlers.erase(it);
		}
	}

	template<typename EventT>
	inline void EventDispatcher::DeregisterEventHandler(const ComponentHandleBase& handle)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		auto eventType = std::type_index(typeid(EventT));

		auto it = eventHandlers.find(eventType);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << eventType.name() << std::endl;
			return;
		}

		it->second.RemoveByListener(&handle);

		if (it->second.IsEmpty())
		{
			eventHandlers.erase(it);
		}
	}

	template<typename EventT>
	inline void EventDispatcher::DeregisterEventHandler(const ObjectHandleBase& handle)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		auto eventType = std::type_index(typeid(EventT));

		auto it = eventHandlers.find(eventType);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << eventType.name() << std::endl;
			return;
		}

		it->second.RemoveByListener(&handle);

		if (it->second.IsEmpty())
		{
			eventHandlers.erase(it);
		}
	}

	template<typename EventT, typename T>
	inline void EventDispatcher::DeregisterEventHandler(const std::shared_ptr<T>& instance)
	{
		static_assert(std::is_base_of_v<DispatchableEvent, EventT>, "EventT must derive from DispatchableEvent");

		auto eventType = std::type_index(typeid(EventT));

		auto it = eventHandlers.find(eventType);

		if (it == eventHandlers.end())
		{
			std::cout << "No handler registered for event type: " << eventType.name() << std::endl;
			return;
		}

		it->second.RemoveByListener(&instance);

		if (it->second.IsEmpty())
		{
			eventHandlers.erase(it);
		}
	}
}