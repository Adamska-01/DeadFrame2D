#pragma once
#include "CoreEvents/EventManager.h"
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/SceneSystem/SceneManager.h"
#include "Framerate/FrameTimer.h"
#include "SubSystems/SubSystems.h"


namespace DeadFrame2D::Core
{
	// TODO: Change the name of the class so it is different from the DeadFrame2D::Engine namespace
	class DF2D_API Engine
	{
	private:
		std::optional<int> RenderSplashScreen();


	protected:
		std::unique_ptr<SubSystems> engineSubSystems;

		std::unique_ptr<DeadFrame2D::Engine::SceneManager> sceneManager;

		EventManager eventManager;

		FrameTimer frameTimer;


	public:
		Engine();

		virtual ~Engine() = default;


		std::optional<int> Run();
	};
}