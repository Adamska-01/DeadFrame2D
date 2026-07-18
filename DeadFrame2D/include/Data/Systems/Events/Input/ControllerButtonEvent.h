#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Models/Input/Controls/ControllerButtonCode.h"


namespace DF2D::Data
{
	struct ControllerButtonEvent
	{
		InputDeviceID deviceID = 0;

		Models::ControllerButtonCode button;

		bool pressed = false;
	};
}