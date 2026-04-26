#pragma once
#include "Data/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Core
{
	class InputDevice;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API DeviceRemovedEvent : public DispatchableEvent
	{
	private:
		std::string deviceName;

		DeadFrame2D::Data::InputDeviceID deviceID;


	public:
		DeviceRemovedEvent(DeadFrame2D::Data::InputDeviceID deviceID, const std::string& deviceName);


		const std::string& GetDeviceName() const;

		DeadFrame2D::Data::InputDeviceID GetDeviceID() const;
	};
}