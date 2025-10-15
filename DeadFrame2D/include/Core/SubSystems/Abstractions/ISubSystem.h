#pragma once
#include "DF2D_API.h"


namespace DeadFrame2D::Core
{
	class DF2D_API ISubSystem
	{
		friend class SubSystemManager;


	protected:
		virtual ~ISubSystem() = default;


		virtual void Update(float deltaTime) = 0;

		virtual void BeginFrame() = 0;

		virtual void EndUpdate() = 0;

		virtual void EndDraw() = 0;
	};
}