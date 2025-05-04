#include "Engine/Engine.h"
#include "SubSystems/Input/InputControls.h"


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

		engineSubSystems->BeginFrame();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		sceneManager->UpdateScene(ft.DeltaTime());
		sceneManager->DrawScene();

		sceneManager->LoadNewSceneIfAvailable();
		
		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}