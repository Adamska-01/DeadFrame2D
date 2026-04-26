#include "Engine/Events/SubSystems/Input/DeviceAddedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;


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