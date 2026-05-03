#pragma once
#include "Core/Services/Events/EventManager.h"
#include "Core/Services/Time/FrameTimer.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Scene/SceneManager.h"


namespace DF2D::Core
{
	class SystemInitializer;
	class ServiceInitializer;


	class DF2D_API DeadFrameRuntime
	{
	private:
		std::optional<int> RenderSplashScreen();


	protected:
		std::unique_ptr<SystemInitializer> systemInitializer;

		std::unique_ptr<ServiceInitializer> serviceInitializer;


	public:
		DeadFrameRuntime();

		virtual ~DeadFrameRuntime();


		std::optional<int> Run();
	};
}