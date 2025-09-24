#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"


namespace DeadFrame2D::Core
{
	class KeyboardInputDevice : public InputDevice
	{
	public:
		KeyboardInputDevice();


		virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
	};
}