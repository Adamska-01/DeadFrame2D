#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API ControllerDisconnectedEvent : public DispatchableEvent
	{
	public:
		const DeadFrame2D::Core::InputDevice* controllerDevice;


		ControllerDisconnectedEvent(const DeadFrame2D::Core::InputDevice* controllerDevice);
	};
}