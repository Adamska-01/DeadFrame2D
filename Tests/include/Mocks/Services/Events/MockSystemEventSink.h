#pragma once
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include <vector>


struct MockSystemEventSink : DF2D::Core::ISystemEventSink
{
	std::vector<DF2D::Data::SystemEvent> receivedEvents;


	void OnSystemEvent(const DF2D::Data::SystemEvent& systemEvent) override
	{
		receivedEvents.push_back(systemEvent);
	}
};
