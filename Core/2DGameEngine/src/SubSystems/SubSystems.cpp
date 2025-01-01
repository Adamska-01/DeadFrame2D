#include "Constants/ScreenConstants.h"
#include "SubSystems/Input.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/SubSystems.h"
#include "SubSystems/Window.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


Window* SubSystems::window = nullptr;

Renderer* SubSystems::renderer = nullptr;

Input* SubSystems::input = nullptr;


SubSystems::~SubSystems()
{
	delete renderer;
	renderer = nullptr;

	delete window;
	window = nullptr;

	delete input;
	input = nullptr;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_CloseAudio();
}

void SubSystems::InitializeSubSystems()
{
	window = new Window(
		ScreenConstants::DEFAULT_SCREENWIDTH,
		ScreenConstants::DEFAULT_SCREENHEIGHT,
		"App");

	renderer = new Renderer(window->GetWindow());

	input = new Input();

	// SDL_Image
	auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
	{
		std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;
	}

	// SDL_TTF
	if (TTF_Init() < 0)
	{
		std::cerr << "Failed to initialize SDL_ttf: " << SDL_GetError() << std::endl;
	}

	// SDL_Mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
	{
		std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}