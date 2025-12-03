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
	class DF2D_API DeviceRemovedEvent : public DispatchableEvent
	{
	private:
		std::shared_ptr<const DeadFrame2D::Core::InputDevice> deviceRemoved;


	public:
		DeviceRemovedEvent(std::shared_ptr<DeadFrame2D::Core::InputDevice> controllerDevice);


		std::shared_ptr<const DeadFrame2D::Core::InputDevice> GetDeviceRemoved() const;
	};
}