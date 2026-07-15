#include "Constants/Input/DefaultDeviceIDs.h"
#include "Constants/Input/DefaultDeviceNames.h"
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	KeyboardInputDevice::KeyboardInputDevice(IInputActionHandler* actionHandler)
		: InputDevice(DefaultDeviceNames::KEYBOARD, actionHandler)
	{
		states = std::vector<InputControlState>(static_cast<size_t>(KeyboardKeyCode::COUNT_MAX));
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

	void KeyboardInputDevice::HandleKey(KeyboardKeyCode key, bool pressed)
	{
		auto controlID = static_cast<uint16_t>(key);

		if (controlID >= states.size())
			return;

		auto& state = states[controlID];

		if (pressed)
		{
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
		}
		else
		{
			state.value = 0.0f;
			state.pressed = false;
			state.held = false;
			state.released = true;

			activeControlIDs.insert(controlID);

			actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
		}
	}

	InputControlState KeyboardInputDevice::GetButtonState(int buttonID) const
	{
		if (buttonID < 0 || buttonID >= static_cast<int>(states.size()))
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
		return GetButtonState(static_cast<int>(code));
	}
}