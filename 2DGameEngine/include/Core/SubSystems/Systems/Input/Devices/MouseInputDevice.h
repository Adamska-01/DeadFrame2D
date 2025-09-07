#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"



class MouseInputDevice : public InputDevice
{
public:
	MouseInputDevice() = default;


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};