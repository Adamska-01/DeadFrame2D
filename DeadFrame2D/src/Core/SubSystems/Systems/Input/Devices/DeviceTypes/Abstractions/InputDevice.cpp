#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"


namespace DeadFrame2D::Core
{
	InputDevice::InputDevice(const char* name)
		: name(name)
	{
	}

	const std::string& InputDevice::Name() const
	{ 
		return name; 
	}
}