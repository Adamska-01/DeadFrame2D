#if DEBUG
#include <cassert>
#endif
#include "Constants/Paths/ResourcePaths.h"
#include "Factories/Products/Context/Systems/Window/SDLWindowBackend.h"
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <set>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Constants;


	namespace
	{
		std::vector<SDL_DisplayMode> GetSupportedResolutions(SDL_Window* window)
		{
			std::vector<SDL_DisplayMode> resolutions;
			std::set<std::pair<int, int>> uniqueResolutions;

			auto displayIndex = SDL_GetWindowDisplayIndex(window);
			if (displayIndex < 0)
			{
				std::cerr << "SDL_GetWindowDisplayIndex Error: " << SDL_GetError() << std::endl;
				return resolutions;
			}

			auto numModes = SDL_GetNumDisplayModes(displayIndex);
			if (numModes < 0)
			{
				std::cerr << "SDL_GetNumDisplayModes Error: " << SDL_GetError() << std::endl;
				return resolutions;
			}

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
	}


	SDLWindowBackend::SDLWindowBackend(const std::string& title, int width, int height, bool fullscreen)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Failed to Initialize SDL_VIDEO: " << SDL_GetError() << std::endl;
		}

		auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

		if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
		{
			std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;
		}
		else
		{
			std::cout << "[Info] SDL_image successfully initialized." << std::endl;
		}

		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			0);

#if DEBUG
		assert(window && SDL_GetError());
#endif

		SetWindowIcon(Paths::Files::WINDOW_ICON);

		if (fullscreen)
		{
			SetWindowMode(Data::WindowMode::WINDOW_FULLSCREEN);

			auto maxRes = GetSupportedResolutions(window).front();

			SetResolution(Vector2I(maxRes.w, maxRes.h));
		}

		std::cout << "[Info] Window backend successfully initialized." << std::endl;
	}

	SDLWindowBackend::~SDLWindowBackend()
	{
		if (window == nullptr)
		{
			std::cout << "[Info] Window backend is already nullptr, nothing to destroy." << std::endl;

			return;
		}

		SDL_DestroyWindow(window);

		SDL_Quit();

		std::cout << "[Info] Window backend successfully destroyed." << std::endl;
	}


	Vector2I SDLWindowBackend::GetResolution()
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

	void SDLWindowBackend::SetWindowTitle(const std::string& title)
	{
		if (window == nullptr)
		{
			std::cerr << "[Error] Cannot set window title: window is nullptr." << std::endl;

			return;
		}

		SDL_SetWindowTitle(window, title.c_str());

		std::cout << "[Info] Window title changed to: " << title << std::endl;
	}

	void SDLWindowBackend::SetWindowIcon(std::string_view iconSource)
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

	void SDLWindowBackend::SetWindowMode(Data::WindowMode mode)
	{
		switch (mode)
		{
		case Data::WindowMode::WINDOWED:
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowBordered(window, SDL_TRUE);
			break;

		case Data::WindowMode::BORDERLESS:
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowBordered(window, SDL_FALSE);
			break;

		case Data::WindowMode::WINDOW_FULLSCREEN:
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;

		case Data::WindowMode::FULLSCREEN:
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			break;

		default:
			break;
		}
	}

	bool SDLWindowBackend::SetResolution(Vector2I resolution)
	{
		auto supportedResolutions = GetSupportedResolutions(window);

		auto it = std::find_if(
			supportedResolutions.begin(),
			supportedResolutions.end(),
			[resolution](const SDL_DisplayMode& mode)
			{
				return mode.w == resolution.x && mode.h == resolution.y;
			});

		if (it == supportedResolutions.end())
			return false;

		SDL_SetWindowSize(window, resolution.x, resolution.y);

		SDL_DisplayMode mode{ 0 };
		mode.w = resolution.x;
		mode.h = resolution.y;

		SDL_SetWindowDisplayMode(window, &mode);

		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		return true;
	}

	SDL_Window* SDLWindowBackend::GetSDLWindow()
	{
		return window;
	}
}