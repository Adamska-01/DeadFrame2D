#include "SubSystems/Events/EventManager.h"
#include <algorithm>
#include <SDL.h>


std::vector<IEventProcessor*> EventManager::eventProcessors = {};


EventManager::EventManager()
{
	sdlEvent = SDL_Event();
}

std::optional<int> EventManager::ProcessEvents()
{
	while (SDL_PollEvent(&sdlEvent))
	{
		for (const auto& processor : eventProcessors)
		{
			if (const auto ecode = processor->ProcessEvents(sdlEvent))
				return ecode;
		}
	}

	return std::nullopt;
}

void EventManager::AddEventProcessor(IEventProcessor* eventProcessor)
{
	eventProcessors.push_back(eventProcessor);
}

void EventManager::RemoveEventProcessor(IEventProcessor* eventProcessor)
{
	auto it = std::remove(
		eventProcessors.begin(), 
		eventProcessors.end(), 
		eventProcessor);

	eventProcessors.erase(it, eventProcessors.end());
}