#pragma once
#include "DF2D_API.h"
#include "Models/Input/ActionMap/Types/InputControlType.h"


namespace DF2D::Core
{
	class InputDevice;


	class DF2D_API IInputActionHandler
	{
	public:
		virtual ~IInputActionHandler() = default;


		virtual void ProcessBinding(const InputDevice& device, Models::InputControlType inputControlType, int controlID) = 0;
	};
}