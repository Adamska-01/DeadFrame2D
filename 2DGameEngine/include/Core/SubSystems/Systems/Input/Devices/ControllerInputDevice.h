#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"


namespace DeadFrame2D::Core
{
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
}