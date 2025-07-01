#include "Debugging/Debug.h"
#include "SubSystems/Window.h"
#include <algorithm>
#include <set>
#include <SDL_image.h>


SDL_Window* Window::window = nullptr;


Window::Window(WindowConfig windowConfig)
{
	window = SDL_CreateWindow(
		windowConfig.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowConfig.width,
		windowConfig.height,
		// Window behaviour flag (not used "0")
		0);

#if _DEBUG
	DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
#endif

	SetWindowIcon(windowConfig.iconSourcePath);

	if (windowConfig.fullscreen)
	{
		SetWindowMode(WindowMode::BORDERLESS);

		auto maxRes = GetSupportedResolutions().back();

		SetResolution(Vector2I(maxRes.w, maxRes.h));
	}

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

void Window::Update(float deltaTime)
{

}

void Window::BeginFrame()
{

}

void Window::EndUpdate()
{

}

void Window::EndDraw()
{

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

void Window::SetWindowIcon(std::string_view iconSource)
{
	if (window == nullptr)
	{
		std::cerr << "[Error] Cannot set icon, window is nullptr." << std::endl;
		
		return;
	}

	auto iconSurface = IMG_Load(std::string(iconSource).c_str());
	
	if (iconSurface == nullptr)
	{
		std::cerr << "[Error] Failed to load window icon from '" << iconSource << "': " << IMG_GetError() << std::endl;
	
		return;
	}

	SDL_SetWindowIcon(window, iconSurface);
	SDL_FreeSurface(iconSurface);

	std::cout << "[Info] Window icon set from '" << iconSource << "'." << std::endl;
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

	case WindowMode::WINDOW_FULLSCREEN:
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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