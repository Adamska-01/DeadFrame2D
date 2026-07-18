#include "Constants/Input/InputConstants.h"
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include <cmath>


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	ControllerInputDevice::ControllerInputDevice(InputDeviceID instanceID, const std::string& name, IInputActionHandler* actionHandler)
		: InputDevice(name.c_str(), actionHandler),
		instanceID(instanceID)
	{
		buttonStates = std::vector<InputControlState>(static_cast<size_t>(ControllerButtonCode::COUNT_MAX));
		axisStates = std::vector<InputControlState>(static_cast<size_t>(ControllerAxisCode::COUNT_MAX));
	}


	void ControllerInputDevice::BeginFrame()
	{
		for (auto button : activeButtons)
		{
			auto& state = buttonStates[button];

			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, button);
			}

			if (state.released)
			{
				state.released = false;
				state.value = 0.0f;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, button);
			}
		}

		for (auto axis : activeAxes)
		{
			auto& state = axisStates[axis];

			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionHandler->ProcessBinding(*this, InputControlType::ANALOG, axis);
			}

			if (state.released)
			{
				state.released = false;
				state.value = 0.0f;

				actionHandler->ProcessBinding(*this, InputControlType::ANALOG, axis);
			}
		}

		activeButtons.clear();
		activeAxes.clear();
	}

	bool ControllerInputDevice::HandleEvent(const SystemEvent& systemEvent)
	{
		return std::visit(
			[&](const auto& event) -> bool
			{
				using T = std::decay_t<decltype(event)>;

				if constexpr (std::is_same_v<T, ControllerButtonEvent>)
				{
					if (event.deviceID != instanceID)
						return false;

					HandleButton(event.button, event.pressed);

					return true;
				}
				else if constexpr (std::is_same_v<T, ControllerAxisEvent>)
				{
					if (event.deviceID != instanceID)
						return false;

					HandleAxis(event.axis, event.value);

					return true;
				}
				else
					return false;
			},
			systemEvent);
	}

	void ControllerInputDevice::HandleButton(ControllerButtonCode button, bool pressed)
	{
		auto controlID = static_cast<int>(button);

		if (controlID < 0 || controlID >= static_cast<int>(buttonStates.size()))
			return;

		auto& state = buttonStates[controlID];

		if (pressed)
		{
			if (!state.held && !state.pressed)
			{
				state.value = 1.0f;
				state.pressed = true;
				state.held = false;
				state.released = false;

				activeButtons.insert(controlID);

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}
		}
		else
		{
			state.value = 0.0f;
			state.pressed = false;
			state.held = false;
			state.released = true;

			activeButtons.insert(controlID);

			actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
		}
	}

	void ControllerInputDevice::HandleAxis(ControllerAxisCode axis, float normalizedValue)
	{
		auto controlID = static_cast<int>(axis);

		if (controlID < 0 || controlID >= static_cast<int>(axisStates.size()))
			return;

		auto isHeldNow = std::fabs(normalizedValue) >= InputConstants::ANALOG_PRESS_THRESHOLD;

		auto& state = axisStates[controlID];

		auto wasHeld = state.held;

		state.value = normalizedValue;
		state.held = isHeldNow;
		state.pressed = !wasHeld && isHeldNow;
		state.released = wasHeld && !isHeldNow;

		activeAxes.insert(controlID);

		actionHandler->ProcessBinding(*this, InputControlType::ANALOG, controlID);
	}

	InputControlState ControllerInputDevice::GetButtonState(int buttonID) const
	{
		if (buttonID >= 0 && buttonID<static_cast<int>(buttonStates.size()))
			return buttonStates[buttonID];

		return {};
	}

	InputControlState ControllerInputDevice::GetAxisState(int axisID) const
	{
		if (axisID >= 0 && axisID<static_cast<int>(axisStates.size()))
			return axisStates[axisID];

		return {};
	}


	InputDeviceType ControllerInputDevice::Type() const
	{
		return InputDeviceType::CONTROLLER;
	}

	InputDeviceID ControllerInputDevice::ID() const
	{
		return instanceID;
	}

	InputControlState ControllerInputDevice::GetButtonState(ControllerButtonCode code) const
	{
		return GetButtonState(static_cast<int>(code));
	}

	InputControlState ControllerInputDevice::GetAxisState(Models::ControllerAxisCode code) const
	{
		return GetAxisState(static_cast<int>(code));
	}
}