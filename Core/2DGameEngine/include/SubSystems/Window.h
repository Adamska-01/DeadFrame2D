#pragma once
#include "Generic/WindowMode.h"
#include "Math/Vector2.h"
#include "SubSystems/Events/Interfaces/IEventProcessor.h"
#include <string>
#include <vector>


class Window : public IEventProcessor
{
private:
	static SDL_Window* window;
	

public:
	Window(int width, int height, const char* title);

	~Window();


	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	static SDL_Window* GetWindow();

	static Vector2I GetResolution();

	static std::vector<SDL_DisplayMode> GetSupportedResolutions();

	static void SetWindowMode(WindowMode mode);

	static bool SetResolution(Vector2I resolution);
};