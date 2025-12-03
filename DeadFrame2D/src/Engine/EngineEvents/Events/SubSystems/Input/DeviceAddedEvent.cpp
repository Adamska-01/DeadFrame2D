#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceAddedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	DeviceAddedEvent::DeviceAddedEvent(std::shared_ptr<InputDevice> deviceAdded)
		: deviceAdded(std::move(deviceAdded))
	{
	}

	std::shared_ptr<const InputDevice> DeviceAddedEvent::GetDeviceAdded() const
	{
		return deviceAdded;
	}
}