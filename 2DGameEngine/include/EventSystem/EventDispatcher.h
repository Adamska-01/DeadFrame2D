#pragma once
#include "DispatchableEvent.h"
#include "Tools/MulticastDelegate.h"
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
	static void RegisterEventHandler(const std::type_index& eventType, const std::function<void(std::shared_ptr<DispatchableEvent>)>& handler, std::uintptr_t identifier);

	static void DeregisterEventHandler(const std::type_index& eventType, std::uintptr_t identifier);
	
	static void SendEvent(std::shared_ptr<DispatchableEvent> event);
};