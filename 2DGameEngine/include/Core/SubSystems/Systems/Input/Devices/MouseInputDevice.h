#pragma once
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "DF2D_API.h"


namespace DeadFrame2D::Core
{
	class DF2D_API MouseInputDevice : public InputDevice
	{
	public:
		MouseInputDevice() = default;


		virtual void ProcessEvent(const SDL_Event& controllerEvent) override;
	};
}