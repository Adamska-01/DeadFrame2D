#pragma once
#include "Data/Systems/Input/InputDeviceID.h"


namespace DF2D::Data
{
	struct ControllerDisconnectedEvent
	{
		InputDeviceID deviceID = 0;
	};
}