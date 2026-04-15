#include "Constants/Input/DefaultDeviceIDs.h"
#include "Constants/Input/DefaultDeviceNames.h"
#include "Converters/Input/KeyboardKeyCodeConversions.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include <SDL_events.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Internal;
	using namespace DeadFrame2D::Models;


	KeyboardInputDevice::KeyboardInputDevice(IInputActionHandler* actionHandler)
		: InputDevice(DefaultDeviceNames::KEYBOARD, actionHandler)
	{
		states = std::vector<DeadFrame2D::Data::InputControlState>(SDL_NUM_SCANCODES);
	}


	void KeyboardInputDevice::BeginFrame()
	{
		for (auto& controlID : activeControlIDs)
		{
			auto& state = states[controlID];

			// pressed is true only the first frame, then becomes held
			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}

			// released is true only the frame the key was released
			if (state.released)
			{
				state.released = false;
			
				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}
		}

		activeControlIDs.clear();
	}

	void KeyboardInputDevice::ProcessEvent(const SDL_Event& event)
	{
		auto eventType = event.type;

		switch (eventType)
		{
			case SDL_EventType::SDL_KEYDOWN:
			{
				auto controlID = event.key.keysym.scancode;
				auto& state = states[controlID];

				// Only set pressed if it wasn't already held
				if (!state.held && !state.pressed)
				{
					state.value = 1.0f;
					state.pressed = true;
					state.held = false;
					state.released = false;

					activeControlIDs.insert(controlID);

					actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
				}

				break;
			}
			case SDL_EventType::SDL_KEYUP:
			{
				auto controlID = event.key.keysym.scancode;
				auto& state = states[controlID];

				state.value = 0.0f;
				state.pressed = false;
				state.held = false;
				state.released = true;

				activeControlIDs.insert(controlID);
				
				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);

				break;
			}

			default:
				break;
		}
	}

	InputControlState KeyboardInputDevice::GetButtonState(int buttonID) const
	{
		if (buttonID < 0 || buttonID >= SDL_Scancode::SDL_NUM_SCANCODES)
			return {};

		return states[buttonID];
	}

	InputControlState KeyboardInputDevice::GetAxisState(int axisID) const
	{
		// Axes are user-defined on keyboard (actions)
		return {};
	}


	InputDeviceType KeyboardInputDevice::Type() const
	{
		return InputDeviceType::KEYBOARD;
	}

	InputDeviceID KeyboardInputDevice::ID() const
	{
		return DefaultDeviceIDs::KEYBOARD;
	}

	InputControlState KeyboardInputDevice::GetButtonState(KeyboardKeyCode code) const
	{
		auto sdlCode = static_cast<int>(KeyboardKeyCodeConversions::ToSDLScancode(code));

		return GetButtonState(sdlCode);
	}
}