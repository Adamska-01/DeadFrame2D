#pragma once
#include "DispatchableEvent.h"
#include "Tools/MulticastDelegate.h"
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>


using EventHandler = MulticastDelegate<std::shared_ptr<DispatchableEvent>>;


class EventDispatcher
{
private:
	static std::unordered_map<std::type_index, EventHandler> eventHandlers;


	// Prevent instantiation
	EventDispatcher() = delete;


public:
	static void RegisterEventHandler(const std::type_index& eventType, const EventHandler& handler);

	static void DeregisterEventHandler(const std::type_index& eventType, const EventHandler& handler);
	
	static void SendEvent(std::shared_ptr<DispatchableEvent> event);
};