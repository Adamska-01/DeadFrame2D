#pragma once
#include "SubSystems/Input/Devices/InputDevice.h"


class MouseInputDevice : public InputDevice
{
public:
	MouseInputDevice() = default;


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};