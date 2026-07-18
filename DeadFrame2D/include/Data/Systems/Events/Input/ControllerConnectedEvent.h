#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include <string>


namespace DF2D::Data
{
	struct ControllerConnectedEvent
	{
		InputDeviceID deviceID = 0;

		std::string name;
	};
}