#pragma once
#include "CoreEvents/EventManager.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/SceneSystem/SceneManager.h"
#include "Framerate/FrameTimer.h"
#include "SubSystems/SubSystems.h"
#include <Models/Engine/EngineConfig.h>


namespace DeadFrame2D::Core
{
	// TODO: Change the name of the class so it is different from the DeadFrame2D::Engine namespace
	class Engine
	{
	private:
		std::optional<int> RenderSplashScreen();


	protected:
		std::unique_ptr<SubSystems> engineSubSystems;

		std::unique_ptr<DeadFrame2D::Engine::SceneManager> sceneManager;

		EventManager eventManager;

		FrameTimer frameTimer;

		Shared::Models::EngineConfig engineConfig;


	public:
		Engine();
	
		virtual ~Engine() = default;


		std::optional<int> Run();
	};
}