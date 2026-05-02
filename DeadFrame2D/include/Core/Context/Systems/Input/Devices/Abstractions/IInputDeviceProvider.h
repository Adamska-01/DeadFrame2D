#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <vector>


namespace DeadFrame2D::Core
{
	class InputDevice;
	class KeyboardInputDevice;
	class MouseInputDevice;
	class ControllerInputDevice;


	class DF2D_API IInputDeviceProvider
	{
	public:
		virtual ~IInputDeviceProvider() = default;


		virtual InputDevice* GetDevice(DeadFrame2D::Data::InputDeviceID id) = 0;

		virtual std::vector<InputDevice*> GetAllDevices() const = 0;

		virtual KeyboardInputDevice* Keyboard() = 0;

		virtual MouseInputDevice* Mouse() = 0;

		virtual ControllerInputDevice* Controller(DeadFrame2D::Data::InputDeviceID id) = 0;

		virtual ControllerInputDevice* CurrentController() = 0;
	};
}