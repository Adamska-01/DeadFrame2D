#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"


class KeyboardInputDevice : public InputDevice
{
public:
	KeyboardInputDevice();


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};