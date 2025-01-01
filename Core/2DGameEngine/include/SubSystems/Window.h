#pragma once
#include "Generic/WindowMode.h"
#include "Math/Vector2.h"
#include "SubSystems/Events/Interfaces/IEventProcessor.h"
#include <string>


class Window : public IEventProcessor
{
private:
	static SDL_Window* window;
	
	static int width;

	static int height;


public:
	Window(int width, int height, const char* title);

	~Window();


	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	static SDL_Window* GetWindow();

	static Vector2 GetWindowSize();

	static void SetWindowMode(SDL_Window* window, WindowMode mode);
};