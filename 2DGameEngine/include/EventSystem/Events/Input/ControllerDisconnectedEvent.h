#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "SubSystems/Input/Devices/InputDevice.h"


class ControllerDisconnectedEvent : public DispatchableEvent
{
public:
	const InputDevice* controllerDevice;


	ControllerDisconnectedEvent(const InputDevice* controllerDevice);
};