#include "Engine/Engine.h"
#include "SubSystems/Renderer.h"
#include <SubSystems/Input/InputControls.h>


Engine::Engine()
{
	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems();

	gameObjectRegistry = std::make_unique<GameObjectRegistry>();

	ft.SetTargetFramerate(60);

	InputControls::Deserialize("../Core/2DGameEngine/include/InputControls.json");
}

void Engine::Update(float deltaTime)
{
	gameObjectRegistry->Update(deltaTime);
}

void Engine::Draw()
{
	Renderer::ClearBuffer();

	gameObjectRegistry->Draw();

	Renderer::PresentBuffer();
}

std::optional<int> Engine::Run()
{
	while (true)
	{
		ft.StartClock();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		Update(ft.DeltaTime());
		Draw();

		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}