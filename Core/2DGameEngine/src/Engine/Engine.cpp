#include "Engine/Engine.h"
#include "SubSystems/Renderer.h"


Engine::Engine()
{
	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems();

	sceneManager = std::make_unique<SceneManager>();
	
	ft.SetTargetFramerate(60);
}

std::optional<int> Engine::Run()
{
	while (true)
	{
		ft.StartClock();

		auto deltaTime = ft.DeltaTime();

		engineSubSystems->BeginFrame();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		engineSubSystems->Update(deltaTime);
		
		sceneManager->UpdateScene(deltaTime);

		Renderer::ClearBuffer();

		sceneManager->DrawScene();

		engineSubSystems->EndFrame();

		Renderer::PresentBuffer();

		sceneManager->LoadNewSceneIfAvailable();

		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}