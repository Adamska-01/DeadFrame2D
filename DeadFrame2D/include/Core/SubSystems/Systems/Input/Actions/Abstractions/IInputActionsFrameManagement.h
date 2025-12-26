#pragma once
#include "DF2D_API.h"


namespace DeadFrame2D::Core
{
	class InputDevice;


	class DF2D_API IInputActionsFrameManagement
	{
	public:
		virtual ~IInputActionsFrameManagement() = default;


		virtual void ProcessBinding(const InputDevice& device, int controlID) = 0;

		virtual void BeginFrame() = 0;

		virtual void FinalizeActions() = 0;
	};
}