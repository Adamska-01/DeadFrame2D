#pragma once
#include "CoreEvents/EventManager.h"
#include "DF2D_API.h"
#include "Engine/SceneSystem/SceneManager.h"
#include "Framerate/FrameTimer.h"
#include "SubSystems/SubSystemManager.h"


namespace DeadFrame2D::Core
{
	class DF2D_API DeadFrameRuntime
	{
	private:
		std::optional<int> RenderSplashScreen();


	protected:
		std::unique_ptr<SubSystemManager> engineSubSystems;

		std::unique_ptr<DeadFrame2D::Engine::SceneManager> sceneManager;

		EventManager eventManager;

		FrameTimer frameTimer;


	public:
		DeadFrameRuntime();

		virtual ~DeadFrameRuntime() = default;


		std::optional<int> Run();
	};
}