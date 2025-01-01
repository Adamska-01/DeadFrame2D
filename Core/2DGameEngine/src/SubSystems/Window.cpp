#include "Subsystems/Events/EventManager.h"
#include "Subsystems/Window.h"
#include <Constants/ScreenConstants.h>
#include <Debugging/Debug.h>


SDL_Window* Window::window = nullptr;

int Window::width = 0;

int Window::height = 0;


Window::Window(int width, int height, const char* title)
{
	this->width = width;
	this->height = height;

	window = SDL_CreateWindow(
		title,
		ScreenConstants::INITIAL_SCREEN_X_POS,
		ScreenConstants::INITIAL_SCREEN_Y_POS,
		width,
		height,
		// Window behaviour flag (not used "0")
		0);

#if _DEBUG
	DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
#endif

	EventManager::AddEventProcessor(this);
}

Window::~Window()
{
	if (window == nullptr)
		return;

	SDL_DestroyWindow(window);
}

std::optional<int> Window::ProcessEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_QUIT:
		return (int)SDL_QUIT;

	case SDL_WINDOWEVENT_CLOSE:
		return (int)SDL_WINDOWEVENT_CLOSE;

	default:
		break;
	}

	return std::nullopt;
}

SDL_Window* Window::GetWindow()
{
	return window;
}

Vector2 Window::GetWindowSize()
{
	return Vector2(width, height);
}

void Window::SetWindowMode(SDL_Window* window, WindowMode mode)
{
	// First, clear the previous window mode.
	SDL_SetWindowFullscreen(window, 0);
	
	switch (mode)
	{
	case WindowMode::WINDOWED:
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowBordered(window, SDL_TRUE);
		break;

	case WindowMode::BORDERLESS:
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowBordered(window, SDL_FALSE);
		break;

	case WindowMode::FULLSCREEN:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		break;

	default:
		break;
	}

	SDL_GetWindowSize(window, &width, &height);
}