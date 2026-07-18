#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Models/Input/Controls/ControllerAxisCode.h"


namespace DF2D::Data
{
	struct ControllerAxisEvent
	{
		InputDeviceID deviceID = 0;

		Models::ControllerAxisCode axis;

		/** @brief Normalized to [-1, 1]. */
		float value = 0.0f;
	};
}