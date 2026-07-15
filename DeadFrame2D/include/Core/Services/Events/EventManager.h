#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "DF2D_API.h"
#include <memory>
#include <optional>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API EventManager
	{
	private:
		std::unique_ptr<IEventSource> eventSource;

		std::vector<ISystemEventSink*> sinks;


	public:
		EventManager(std::unique_ptr<IEventSource> eventSource);

		// Sinks are not owned; deleting them is up to their owners
		~EventManager() = default;

		EventManager(const EventManager&) = delete;

		EventManager(EventManager&&) = delete;

		EventManager& operator=(const EventManager&) = delete;

		EventManager& operator=(EventManager&&) = delete;


		std::optional<int> ProcessEvents();


		void AddSink(ISystemEventSink* sink);

		void RemoveSink(ISystemEventSink* sink);

		void RequestQuit();
	};
}