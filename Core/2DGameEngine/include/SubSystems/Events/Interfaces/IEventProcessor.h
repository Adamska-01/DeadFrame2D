#pragma once
#include <optional>
#include <SDL.h>


class IEventProcessor
{
public:
	virtual std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) = 0;
};