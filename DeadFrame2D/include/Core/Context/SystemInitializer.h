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
		Data::CoreContext ctx;


	public:
		SystemInitializer(Models::SystemConfig config);

		~SystemInitializer();


		void BeginFrame();

		void PreUpdate(float deltaTime);

		void EndUpdate(float deltaTime);

		void EndDraw();
	};
}