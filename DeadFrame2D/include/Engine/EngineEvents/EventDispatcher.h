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


		// Prevent instantiation
		EventDispatcher() = delete;


	public:
		static void RegisterEventHandler(const std::type_index& eventType, const std::function<void(std::shared_ptr<DispatchableEvent>)>& handler, std::uintptr_t identifier);

		static void DeregisterEventHandler(const std::type_index& eventType, std::uintptr_t identifier);

		static void SendEvent(std::shared_ptr<DispatchableEvent> event);
	};
}