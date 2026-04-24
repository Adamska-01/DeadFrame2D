#pragma once
#include "Core/Context/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include "Models/SystemConfig.h"


namespace DeadFrame2D::Core
{
	class DF2D_API SubSystemManager
	{
	private:
		std::array<ISubSystem*, 8> subSystems;


	public:
		SubSystemManager();

		~SubSystemManager();


		void InitializeSubSystems(Models::SystemConfig config);


		void BeginFrame();

		void PreUpdate(float deltaTime);

		void EndUpdate(float deltaTime);

		void EndDraw();
	};
}