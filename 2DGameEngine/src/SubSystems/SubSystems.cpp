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
{
	subSystems.fill(nullptr);
}

SubSystems::~SubSystems()
{
	for (const auto& subSystem : subSystems)
	{
		delete subSystem;
	}
}

void SubSystems::InitializeSubSystems(EngineConfig config)
{
	// TODO: Create a config file to set the default values for window and renderer 

	auto window = new Window(config.window);

	subSystems[0] = window;

	subSystems[1] = new Renderer(window->GetWindow(), config.rendering);

	subSystems[2] = new Input();

	subSystems[3] = new TextureManager();

	subSystems[4] = new UIManager();

	subSystems[5] = new AudioManager();

	subSystems[6] = new PhysicsEngine2D(Vector2F(PhysicsConstants::GRAVITY_X, PhysicsConstants::GRAVITY_Y));

	auto coroutineScheduler = new CoroutineScheduler();
	CoroutineScheduler::SetCurrent(coroutineScheduler);
	
	subSystems[7] = coroutineScheduler;
}

void SubSystems::Update(float deltaTime)
{
	for (const auto& subSystem : subSystems)
	{
		subSystem->Update(deltaTime);
	}
}

// TODO: Create the interface with BeginFrame and EndFrame
void SubSystems::BeginFrame()
{
	for (const auto& subSystem : subSystems)
	{
		subSystem->BeginFrame();
	}
}

void SubSystems::EndUpdate()
{
	for (const auto& subSystem : subSystems)
	{
		subSystem->EndUpdate();
	}
}

void SubSystems::EndDraw()
{
	for (const auto& subSystem : subSystems)
	{
		subSystem->EndDraw();
	}
}