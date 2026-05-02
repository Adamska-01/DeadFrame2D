#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DF2D::Engine
{
	class DF2D_API DeviceAddedEvent : public DispatchableEvent
	{
	private:
		std::string deviceName;

		Data::InputDeviceID deviceID;


	public:
		DeviceAddedEvent(Data::InputDeviceID deviceID, const std::string& deviceName);


		const std::string& GetDeviceName() const;

		Data::InputDeviceID GetDeviceID() const;
	};
}