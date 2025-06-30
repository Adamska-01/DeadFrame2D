#pragma once
#include "SubSystems/Input/Devices/InputDevice.h"


class KeyboardInputDevice : public InputDevice
{
public:
	KeyboardInputDevice();


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};