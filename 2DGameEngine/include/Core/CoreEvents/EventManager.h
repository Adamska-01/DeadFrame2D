#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include <vector>


namespace DeadFrame2D::Core
{
	class EventManager 
	{
	private:
		SDL_Event sdlEvent;


		static std::vector<IEventProcessor*> eventProcessors;


	public:
		EventManager();

		// Deleting the processors is up to the owners 
		~EventManager() = default;


		std::optional<int> ProcessEvents();


		static void AddEventProcessor(IEventProcessor* eventProcessor);

		static void RemoveEventProcessor(IEventProcessor* eventProcessor);

		static void SendSystemEvent(SDL_EventType eventType);
	};
}