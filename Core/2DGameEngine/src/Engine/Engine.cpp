#include "Engine/Engine.h"
#include "SubSystems/Input/InputControls.h"


Engine::Engine()
{
	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems();

	gameObjectRegistry = std::make_unique<SceneManager>();

	ft.SetTargetFramerate(60);

	InputControls::Deserialize("Shared/Configurations/InputControls.json");
}

std::optional<int> Engine::Run()
{
	while (true)
	{
		ft.StartClock();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		gameObjectRegistry->Update(ft.DeltaTime());
		gameObjectRegistry->Draw();

		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}