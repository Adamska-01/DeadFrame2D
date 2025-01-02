#include "Constants/ScreenConstants.h"
#include "Debugging/Debug.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/Window.h"
#include <SDL.h>


SDL_Renderer* Renderer::renderer = nullptr;


Renderer::Renderer(SDL_Window* window)
{
#if _DEBUG
	DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
#endif

	//startup
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to Initialize SDL_VIDEO: " << SDL_GetError() << std::endl;
	}

	//create the renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED);

#if _DEBUG
	DBG_ASSERT_MSG(renderer, "Renderer initialisation failed: %s\n", SDL_GetError());
#endif

	SetResolutionTarget({ ScreenConstants::DEFAULT_RENDERER_X_SIZE, ScreenConstants::DEFAULT_RENDERER_Y_SIZE });

	std::cout << "[Info] SDL_VIDEO successfully initialized." << std::endl;
}

Renderer::~Renderer()
{
	if (renderer == nullptr) 
	{
		std::cout << "[Info] Renderer is already null, no need to destroy." << std::endl;
	
		return;
	}

	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	std::cout << "[Info] Renderer successfully destroyed and SDL_VIDEO successfully quit." << std::endl;
}

void Renderer::ClearBuffer()
{
	SDL_RenderClear(renderer);
}

void Renderer::PresentBuffer()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* Renderer::GetRenderer()
{
	return renderer;
}

void Renderer::SetDisplayColour(int r, int g, int b, int a)
{
	if (renderer == nullptr)
		return;

	//set background color
	auto result = SDL_SetRenderDrawColor(
		renderer,
		r,
		g,
		b,
		a);

#if _DEBUG
	DBG_ASSERT_MSG(!result, "Background color initialisation failed: %s\n", SDL_GetError());
#endif
}

void Renderer::SetResolutionTarget(Vector2I targetResolution)
{
	auto width = targetResolution.x;
	auto height = targetResolution.y;

	if (width <= 0 || height <= 0)
		return;

	SDL_RenderSetLogicalSize(renderer, width, height);
}