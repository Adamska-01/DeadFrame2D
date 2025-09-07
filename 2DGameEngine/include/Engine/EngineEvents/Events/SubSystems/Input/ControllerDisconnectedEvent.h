#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "Engine/EngineEvents/DispatchableEvent.h"


class ControllerDisconnectedEvent : public DispatchableEvent
{
public:
	const InputDevice* controllerDevice;


	ControllerDisconnectedEvent(const InputDevice* controllerDevice);
};