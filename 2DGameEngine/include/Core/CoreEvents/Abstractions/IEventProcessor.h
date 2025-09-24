#pragma once
#include <optional>
#include <SDL.h>


namespace DeadFrame2D::Core
{
	class IEventProcessor
	{
	public:
		IEventProcessor();

		~IEventProcessor();


		virtual std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) = 0;
	};
}