#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "DF2D_API.h"


namespace DeadFrame2D::Core
{
	class DF2D_API ControllerInputDevice : public InputDevice
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