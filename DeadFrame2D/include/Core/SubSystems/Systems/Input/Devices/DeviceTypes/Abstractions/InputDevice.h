#pragma once
#include "Data/Input/InputControlState.h"
#include "DF2D_API.h"
#include <Models/Input/InputDeviceType.h>
#include <SDL_events.h>
#include <string>


namespace DeadFrame2D::Core
{
	class InputActionResolver;


	// SDL_JoystickID or -1/-2 for keyboard/mouse
	using DeviceID = int32_t;


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


	public:
		InputDevice(const char* name);

		virtual ~InputDevice() = default;


		const std::string& Name() const;


		virtual Shared::Models::InputDeviceType Type() const = 0;

		virtual DeviceID ID() const = 0;


		/**
		 * Snapshot states at the beginning of the frame; called by InputManager. 
		 */
		virtual void BeginFrame(InputActionResolver* inputActionResolver) = 0;

		/**
		 * @brief Called when an SDL_Event relevant to this device arrives.
		 * Device must update its internal ControlState map accordingly.
		 */
		virtual void ProcessEvent(const SDL_Event& event, InputActionResolver* inputActionResolver) = 0;

		/**
		 * Query key state by the integer key id (platform-defined). 
		 */
		virtual DeadFrame2D::Data::InputControlState GetKeyState(int controlID) const = 0;
	};
}