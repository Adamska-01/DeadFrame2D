#include "SubSystems/Events/EventManager.h"
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