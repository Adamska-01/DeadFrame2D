#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "Engine/EngineEvents/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class ControllerDisconnectedEvent : public DispatchableEvent
	{
	public:
		const DeadFrame2D::Core::InputDevice* controllerDevice;


		ControllerDisconnectedEvent(const DeadFrame2D::Core::InputDevice* controllerDevice);
	};
}