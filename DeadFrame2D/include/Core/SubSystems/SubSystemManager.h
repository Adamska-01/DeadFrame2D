#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <Models/SystemConfig.h>


namespace DeadFrame2D::Core
{
	class DF2D_API SubSystemManager
	{
	private:
		std::array<ISubSystem*, 8> subSystems;


	public:
		SubSystemManager();

		~SubSystemManager();


		void InitializeSubSystems(Shared::Models::SystemConfig config);


		void BeginFrame();

		void PreUpdate(float deltaTime);

		void EndUpdate(float deltaTime);

		void EndDraw();
	};
}