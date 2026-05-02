#pragma once
#include "Data/Systems/Input/InputControlState.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include "Models/Input/InputDeviceType.h"
#include <string>


union SDL_Event;


namespace DF2D::Core
{
	class IInputActionHandler;

	/**
	 * @brief Base for all input devices (keyboard, mouse, controller).
	 *
	 * Device instances are identified by a stable SDL_JoystickID for controllers
	 * and by synthetic ids for keyboard/mouse.
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
		 * Snapshot states at the beginning of the frame; called by InputManager. 
		 */
		virtual void BeginFrame() = 0;

		/**
		 * @brief Called when an SDL_Event relevant to this device arrives.
		 * Device must update its internal ControlState map accordingly.
		 */
		virtual void ProcessEvent(const SDL_Event& event) = 0;

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