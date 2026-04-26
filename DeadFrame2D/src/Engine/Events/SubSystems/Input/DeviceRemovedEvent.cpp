#include "Engine/Events/SubSystems/Input/DeviceRemovedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;


	DeviceRemovedEvent::DeviceRemovedEvent(InputDeviceID deviceID, const std::string& deviceName)
		: deviceID(deviceID),
		deviceName(deviceName)
	{
	}

	const std::string& DeviceRemovedEvent::GetDeviceName() const
	{
		return deviceName;
	}

	InputDeviceID DeviceRemovedEvent::GetDeviceID() const
	{
		return deviceID;
	}
}