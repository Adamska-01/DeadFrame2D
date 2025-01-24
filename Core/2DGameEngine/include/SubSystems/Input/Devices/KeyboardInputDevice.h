#pragma once
#include "SubSystems/Input/Devices/InputDevice.h"
#include <unordered_map>


class KeyboardInputDevice : public InputDevice
{
protected:
	std::unordered_map<Uint8, bool> currentMouseInputStates;

	std::unordered_map<Uint8, bool> lastMouseInputStates;


public:
	KeyboardInputDevice();


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};