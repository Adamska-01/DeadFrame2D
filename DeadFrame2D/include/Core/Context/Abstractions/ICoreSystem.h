#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	class DF2D_API ICoreSystem
	{
	public:
		virtual ~ICoreSystem() = default;


		virtual void BeginFrame() = 0;

		virtual void PreUpdate(float deltaTime) = 0;

		virtual void EndUpdate(float deltaTime) = 0;

		virtual void EndDraw() = 0;
	};
}