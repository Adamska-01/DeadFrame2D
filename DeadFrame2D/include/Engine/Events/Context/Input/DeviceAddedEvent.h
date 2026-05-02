#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API DeviceAddedEvent : public DispatchableEvent
	{
	private:
		std::string deviceName;

		DeadFrame2D::Data::InputDeviceID deviceID;


	public:
		DeviceAddedEvent(DeadFrame2D::Data::InputDeviceID deviceID, const std::string& deviceName);


		const std::string& GetDeviceName() const;

		DeadFrame2D::Data::InputDeviceID GetDeviceID() const;
	};
}