#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include <deque>


struct MockEventSource : DF2D::Core::IEventSource
{
	// Scripted event queue
	std::deque<DF2D::Data::SystemEvent> events;


	// Call tracking
	int pollCount{0};

	int pushQuitCount{0};


	std::optional<DF2D::Data::SystemEvent> PollEvent() override
	{
		pollCount++;

		if (events.empty())
			return std::nullopt;

		auto event = events.front();
		events.pop_front();

		return event;
	}

	void PushQuitEvent() override
	{
		pushQuitCount++;
		events.push_back(DF2D::Data::QuitEvent{});
	}
};
