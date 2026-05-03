#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Data/Systems/CoreContext.h"
#include "DF2D_API.h"
#include "Models/SystemConfig.h"


namespace DF2D::Core
{
	class DF2D_API SystemInitializer
	{
	private:
		DF2D::Data::CoreContext ctx;


	public:
		SystemInitializer() = default;

		~SystemInitializer();


		void InitializeSubSystems(Models::SystemConfig config);


		void BeginFrame();

		void PreUpdate(float deltaTime);

		void EndUpdate(float deltaTime);

		void EndDraw();
	};
}