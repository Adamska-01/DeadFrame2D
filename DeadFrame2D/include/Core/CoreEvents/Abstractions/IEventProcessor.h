#pragma once
#include "DF2D_API.h"
#include <optional>


union SDL_Event;


namespace DeadFrame2D::Core
{
	class DF2D_API IEventProcessor
	{
	public:
		IEventProcessor();

		~IEventProcessor();


		virtual std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) = 0;
	};
}