#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	class InputDevice;


	class DF2D_API IInputFrameLifecycle
	{
	public:
		virtual ~IInputFrameLifecycle() = default;


		virtual void BeginFrame() = 0;

		virtual void PreUpdate() = 0;
	};
}