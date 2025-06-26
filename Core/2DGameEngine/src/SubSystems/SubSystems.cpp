#include "Constants/ScreenConstants.h"
#include "Coroutines/CoroutineScheduler.h"
#include "SubSystems/AudioManager.h"
#include "SubSystems/Input/Input.h"
#include "SubSystems/Physics/PhysicsEngine2D.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/SubSystems.h"
#include "SubSystems/TextureManager.h"
#include "SubSystems/UIManager.h"
#include "SubSystems/Window.h"
#include <Constants/PhysicsConstants.h>


SubSystems::SubSystems()
	: renderer(nullptr),
	window(nullptr),
	input(nullptr),
	textureManager(nullptr),
	uiManager(nullptr),
	audioManager(nullptr),
	physicsEngine2D(nullptr),
	coroutineScheduler(nullptr)
{
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
	
	delete physicsEngine2D;
	physicsEngine2D = nullptr;
	
	delete coroutineScheduler;
	coroutineScheduler = nullptr;
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

	physicsEngine2D = new PhysicsEngine2D(Vector2F(PhysicsConstants::GRAVITY_X, PhysicsConstants::GRAVITY_Y));

	coroutineScheduler = new CoroutineScheduler();
	CoroutineScheduler::SetCurrent(coroutineScheduler);
}

void SubSystems::Update(float deltaTime)
{
	coroutineScheduler->Update(deltaTime);
}

// TODO: Create the interface with BeginFrame and EndFrame
void SubSystems::BeginFrame()
{
	input->BeginFrame();
	physicsEngine2D->BeginFrame();
}

void SubSystems::EndUpdate()
{
	physicsEngine2D->EndUpdate();
}

void SubSystems::EndDraw()
{
	physicsEngine2D->EndDraw();
}