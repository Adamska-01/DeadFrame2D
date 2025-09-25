#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include <Models/SystemConfig.h>


namespace DeadFrame2D::Core
{
	// TODO: Rename this to 'SubSystemManager'
	class SubSystems
	{
	private:
		std::array<ISubSystem*, 8> subSystems;


	public:
		SubSystems();

		~SubSystems();


		void InitializeSubSystems(Shared::Models::SystemConfig config);


		void Update(float deltaTime);

		void BeginFrame();

		void EndUpdate();

		void EndDraw();
	};
}