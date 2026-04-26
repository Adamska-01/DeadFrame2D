#pragma once
#include "Core/Context/SystemInitializer.h"
#include "Core/Services/Events/EventManager.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Services/Time/FrameTimer.h"


namespace DeadFrame2D::Core
{
	class DF2D_API DeadFrameRuntime
	{
	private:
		std::optional<int> RenderSplashScreen();


	protected:
		std::unique_ptr<SystemInitializer> engineSubSystems;

		std::unique_ptr<Engine::SceneManager> sceneManager;

		EventManager eventManager;

		FrameTimer frameTimer;


	public:
		DeadFrameRuntime();

		virtual ~DeadFrameRuntime();


		std::optional<int> Run();
	};
}