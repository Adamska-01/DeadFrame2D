#pragma once
#include "Data/Systems/Input/InputControlState.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include "Models/Input/InputDeviceType.h"
#include <string>


namespace DF2D::Core
{
	class IInputActionHandler;

	/**
	 * @brief Base for all input devices (keyboard, mouse, controller).
	 *
	 * Devices are pure state holders fed with engine-typed events; all control
	 * state is keyed by the engine control enums (KeyboardKeyCode, MouseButtonCode, ...).
	 */
	class DF2D_API InputDevice
	{
	protected:
		std::string name;

		IInputActionHandler* actionHandler;


	public:
		InputDevice(const char* name, IInputActionHandler* actionHandler);

		virtual ~InputDevice() = default;


		const std::string& Name() const;


		virtual Models::InputDeviceType Type() const = 0;

		virtual Data::InputDeviceID ID() const = 0;


		/**
		 * Snapshot states at the beginning of the frame; called by the Input system.
		 */
		virtual void BeginFrame() = 0;

		/**
		 * Digital input (keyboard keys, controller buttons).
		 */
		virtual Data::InputControlState GetButtonState(int buttonID) const = 0;

		/**
		 * Analog input (controller sticks, triggers, mouse delta, etc.).
		 */
		virtual Data::InputControlState GetAxisState(int axisID) const = 0;
	};
}