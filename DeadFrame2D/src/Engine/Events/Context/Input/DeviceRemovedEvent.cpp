#include "Engine/Events/Context/Input/DeviceRemovedEvent.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


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