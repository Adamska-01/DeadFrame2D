#include "Engine/Events/Context/Input/DeviceAddedEvent.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	DeviceAddedEvent::DeviceAddedEvent(InputDeviceID deviceID, const std::string& deviceName)
		: deviceName(deviceName),
		deviceID(deviceID)
	{
	}

	const std::string& DeviceAddedEvent::GetDeviceName() const
	{
		return deviceName;
	}

	InputDeviceID DeviceAddedEvent::GetDeviceID() const
	{
		return deviceID;
	}
}