#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "DF2D_API.h"
#include "Models/SystemConfig.h"


namespace DeadFrame2D::Core
{
	class DF2D_API SystemInitializer
	{
	private:
		std::array<ICoreSystem*, 8> subSystems;


	public:
		SystemInitializer();

		~SystemInitializer();


		void InitializeSubSystems(Models::SystemConfig config);


		void BeginFrame();

		void PreUpdate(float deltaTime);

		void EndUpdate(float deltaTime);

		void EndDraw();
	};
}