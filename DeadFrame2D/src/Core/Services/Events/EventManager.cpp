#include "Core/Services/Events/EventManager.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	EventManager::EventManager(std::unique_ptr<IEventSource> eventSource)
		: eventSource(std::move(eventSource))
	{
		Guard::AgainstNull(this->eventSource.get(), NAME_OF(eventSource));
	}

	std::optional<int> EventManager::ProcessEvents()
	{
		while (auto systemEvent = eventSource->PollEvent())
		{
			if (std::holds_alternative<QuitEvent>(*systemEvent))
				return 0;

			for (auto* sink : sinks)
			{
				sink->OnSystemEvent(*systemEvent);
			}
		}

		return std::nullopt;
	}

	void EventManager::AddSink(ISystemEventSink* sink)
	{
		if (sink == nullptr)
			return;

		sinks.push_back(sink);
	}

	void EventManager::RemoveSink(ISystemEventSink* sink)
	{
		sinks.erase(
			std::remove(
				sinks.begin(),
				sinks.end(),
				sink),
			sinks.end());
	}

	void EventManager::RequestQuit()
	{
		eventSource->PushQuitEvent();
	}
}