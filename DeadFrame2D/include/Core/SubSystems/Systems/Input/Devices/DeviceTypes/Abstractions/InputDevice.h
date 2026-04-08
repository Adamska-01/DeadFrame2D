#pragma once
#include "Data/Input/InputControlState.h"
#include "Data/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <Models/Input/InputDeviceType.h>
#include <string>


union SDL_Event;


namespace DeadFrame2D::Core
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


		virtual Shared::Models::InputDeviceType Type() const = 0;

		virtual DeadFrame2D::Data::InputDeviceID ID() const = 0;


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
		virtual DeadFrame2D::Data::InputControlState GetButtonState(int buttonID) const = 0;

		/**
		 * Analog input (controller sticks, triggers, mouse delta, etc.).
		 */
		virtual DeadFrame2D::Data::InputControlState GetAxisState(int axisID) const = 0;
	};
}