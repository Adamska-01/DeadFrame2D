#include <Constants/ScreenConstants.h>
#include <Debugging/Debug.h>
#include <SDL.h>
#include <SubSystems/Renderer.h>


Renderer* Renderer::Instance = nullptr;


Renderer::Renderer()
{
	Window = nullptr;
	renderer = nullptr;

	//startup
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		SDL_Log("Failed to Initialize SDL_VIDEO: %s\n", SDL_GetError());

	//create the window
	Window = SDL_CreateWindow(
		"PuzzleBubbleClone",			//title
		250,							//initial x pos
		50,								//initial y pos
		ScreenConstants::SCREENWIDTH,	//screen width in pixel
		ScreenConstants::SCREENHEIGHT,	//screen height in pixel 
		0);								//window behaviour flag (not used "0")

#if _DEBUG
	DBG_ASSERT_MSG(Window, "Window initialisation failed: %s\n", SDL_GetError());
#endif

	//create the renderer
	renderer = SDL_CreateRenderer(
		Window,
		-1,
		0);

#if _DEBUG
	DBG_ASSERT_MSG(renderer, "Renderer initialisation failed: %s\n", SDL_GetError());
#endif
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

void Renderer::Update()
{
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Renderer::ClearAndPresent()
{
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void Renderer::Delay(float dt)
{
	SDL_Delay(dt);
}

void Renderer::Destroy()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
	}

	if (Window)
	{
		SDL_DestroyWindow(Window);
	}
}