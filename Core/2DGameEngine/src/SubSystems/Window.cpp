#include "Subsystems/Events/EventManager.h"
#include "Subsystems/Window.h"
#include <Constants/ScreenConstants.h>
#include <Debugging/Debug.h>
#include <set>


SDL_Window* Window::window = nullptr;


Window::Window(int width, int height, const char* title)
{
	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		// Window behaviour flag (not used "0")
		0);

#if _DEBUG
	DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
#endif

	std::cout << "[Info] Window successfully initialized." << std::endl;
}

Window::~Window()
{
	if (window == nullptr) 
	{
		std::cout << "[Info] Window is already nullptr, nothing to destroy." << std::endl;

		return;
	}

	SDL_DestroyWindow(window);
	
	std::cout << "[Info] Window successfully destroyed." << std::endl;
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

Vector2I Window::GetResolution()
{
	auto isFullscreen = SDL_GetWindowFlags(window) & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN;

	if (isFullscreen)
	{
		SDL_DisplayMode mode;
		SDL_GetWindowDisplayMode(window, &mode);

		return { mode.w, mode.h };
	}
	else
	{
		auto width = 0;
		auto height = 0;

		SDL_GetWindowSize(window, &width, &height);

		return { width, height };
	}
}

std::vector<SDL_DisplayMode> Window::GetSupportedResolutions()
{
	std::vector<SDL_DisplayMode> resolutions;
	std::set<std::pair<int, int>> uniqueResolutions;

	// Get the display index the window is on
	auto displayIndex = SDL_GetWindowDisplayIndex(window);
	if (displayIndex < 0)
	{
		std::cerr << "SDL_GetWindowDisplayIndex Error: " << SDL_GetError() << std::endl;
		return resolutions;
	}

	// Get the number of supported display modes for the given display
	auto numModes = SDL_GetNumDisplayModes(displayIndex);
	if (numModes < 0)
	{
		std::cerr << "SDL_GetNumDisplayModes Error: " << SDL_GetError() << std::endl;
		return resolutions;
	}

	// Iterate through all available display modes and add them to the vector
	for (auto modeIndex = 0; modeIndex < numModes; ++modeIndex)
	{
		SDL_DisplayMode mode;
		if (SDL_GetDisplayMode(displayIndex, modeIndex, &mode) < 0)
		{
			std::cerr << "SDL_GetDisplayMode Error: " << SDL_GetError() << std::endl;
			continue;
		}

		if (uniqueResolutions.find({ mode.w, mode.h }) == uniqueResolutions.end())
		{
			uniqueResolutions.insert({ mode.w, mode.h });
			resolutions.push_back(mode);
		}
	}

	return resolutions;
}

void Window::SetWindowMode(WindowMode mode)
{	
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
}

bool Window::SetResolution(Vector2I resolution)
{
	auto supportedResolutions = GetSupportedResolutions();

	auto it = std::find_if(
		supportedResolutions.begin(), 
		supportedResolutions.end(), 
		[resolution](const SDL_DisplayMode& mode)
		{
			return mode.w == resolution.x && mode.h == resolution.y;
		});

	if (it == supportedResolutions.end())
		return false;

	// Keep window size and viewport size in sync (fullscreen/windowed)
	SDL_SetWindowSize(window, resolution.x, resolution.y);

	SDL_DisplayMode mode{ 0 };
	mode.w = resolution.x;
	mode.h = resolution.y;

	SDL_SetWindowDisplayMode(window, &mode);

	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	return true;
}