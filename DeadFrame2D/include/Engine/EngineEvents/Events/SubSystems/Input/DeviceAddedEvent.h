#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


namespace DeadFrame2D::Core
{
	class InputDevice;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API DeviceAddedEvent : public DispatchableEvent
	{
	private:
		std::shared_ptr<const DeadFrame2D::Core::InputDevice> deviceAdded;


	public:
		DeviceAddedEvent(std::shared_ptr<DeadFrame2D::Core::InputDevice> deviceAdded);


		std::shared_ptr<const DeadFrame2D::Core::InputDevice> GetDeviceAdded() const;
	};
}