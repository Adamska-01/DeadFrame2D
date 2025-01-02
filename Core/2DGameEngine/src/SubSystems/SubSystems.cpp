#include "Constants/ScreenConstants.h"
#include "SubSystems/AudioManager.h"
#include "SubSystems/Input.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/SubSystems.h"
#include "SubSystems/TextureManager.h"
#include "SubSystems/UIManager.h"
#include "SubSystems/Window.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


SubSystems::SubSystems()
{
	renderer = nullptr;
	window = nullptr;
	input = nullptr;
	textureManager = nullptr;
	uiManager = nullptr;
	audioManager = nullptr;
}

SubSystems::~SubSystems()
{
	delete renderer;
	renderer = nullptr;

	delete window;
	window = nullptr;

	delete input;
	input = nullptr;

	delete textureManager;
	textureManager = nullptr;

	delete uiManager;
	uiManager = nullptr;

	delete audioManager;
	audioManager = nullptr;
}

void SubSystems::InitializeSubSystems()
{
	// TODO: Create a config file to set the default values for window and renderer 

	window = new Window(
		ScreenConstants::DEFAULT_SCREENWIDTH,
		ScreenConstants::DEFAULT_SCREENHEIGHT,
		"App");

	renderer = new Renderer(window->GetWindow());

	input = new Input();

	textureManager = new TextureManager();

	uiManager = new UIManager();

	audioManager = new AudioManager();
}