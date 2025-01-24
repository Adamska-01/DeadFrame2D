#pragma once
#include "SubSystems/Input/Devices/InputDevice.h"


class ControllerInputDevice : public InputDevice
{
protected:
	int deviceID;

	SDL_GameController* gameControllerDevice;


public:
	ControllerInputDevice(int deviceID);
	
	~ControllerInputDevice();


	virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
};