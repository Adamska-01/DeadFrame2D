#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceRemovedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	DeviceRemovedEvent::DeviceRemovedEvent(std::shared_ptr<InputDevice> deviceRemoved)
		: deviceRemoved(deviceRemoved)
	{
	}

	std::shared_ptr<const InputDevice> DeviceRemovedEvent::GetDeviceRemoved() const
	{
		return deviceRemoved;
	}
}