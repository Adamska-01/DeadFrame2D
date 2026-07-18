#include "Constants/Input/DefaultDeviceIDs.h"
#include "Constants/Input/DefaultDeviceNames.h"
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	MouseInputDevice::MouseInputDevice(IInputActionHandler* actionHandler)
		: InputDevice(DefaultDeviceNames::MOUSE, actionHandler)
	{
		buttonStates.fill({});
		axisStates.fill({});
	}


	void MouseInputDevice::BeginFrame()
	{
		for (auto& controlID : activeButtonIDs)
		{
			auto& state = buttonStates[controlID];

			// pressed is true only the first frame, then becomes held
			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}

			// released is true only the frame the button was released
			if (state.released)
			{
				state.released = false;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}
		}

		activeButtonIDs.clear();

		// Delta axes (motion + wheel); position axes are absolute and untouched here
		for (auto i = static_cast<int>(MouseAxisCode::MOTION_X); i <= static_cast<int>(MouseAxisCode::WHEEL_Y); i++)
		{
			auto& state = axisStates[i];

			if (state.value != 0.0f)
			{
				if (!state.held && !state.pressed)
				{
					// first frame of movement
					state.pressed = true;
					state.held = false;
					state.released = false;
				}
				else
				{
					// subsequent frames
					state.pressed = false;
					state.held = true;
					state.released = false;
				}

				actionHandler->ProcessBinding(*this, InputControlType::ANALOG, i);
			}
			else
			{
				if (state.held || state.pressed)
				{
					// first frame of stopping
					state.released = true;
					state.pressed = false;
					state.held = false;

					actionHandler->ProcessBinding(*this, InputControlType::ANALOG, i);
				}
				else
				{
					state.released = false;
				}
			}

			// Reset delta for next frame
			state.value = 0.0f;
		}
	}

	bool MouseInputDevice::HandleEvent(const SystemEvent& systemEvent)
	{
		return std::visit(
			[&](const auto& event) -> bool
			{
				using T = std::decay_t<decltype(event)>;

				if constexpr (std::is_same_v<T, MouseButtonEvent>)
				{
					HandleButton(event.button, event.pressed, event.position);

					return true;
				}
				else if constexpr (std::is_same_v<T, MouseMoveEvent>)
				{
					HandleMove(event.position, event.delta);

					return true;
				}
				else if constexpr (std::is_same_v<T, MouseWheelEvent>)
				{
					HandleWheel(event.delta);

					return true;
				}
				else
					return false;
			},
			systemEvent);
	}

	void MouseInputDevice::HandleButton(MouseButtonCode button, bool pressed, const Vector2F& position)
	{
		auto controlID = static_cast<uint8_t>(button);

		if (controlID >= buttonStates.size())
			return;

		axisStates[static_cast<int>(MouseAxisCode::POSITION_X)].value = position.x;
		axisStates[static_cast<int>(MouseAxisCode::POSITION_Y)].value = position.y;

		auto& state = buttonStates[controlID];

		if (pressed)
		{
			// Only set pressed if it wasn't already held
			if (!state.held && !state.pressed)
			{
				state.value = 1.0f;
				state.pressed = true;
				state.held = false;
				state.released = false;

				activeButtonIDs.insert(controlID);

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}
		}
		else
		{
			state.value = 0.0f;
			state.pressed = false;
			state.held = false;
			state.released = true;

			activeButtonIDs.insert(controlID);

			actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
		}
	}

	void MouseInputDevice::UpdateMotionAxisPhase(MouseAxisCode axis)
	{
		auto& state = axisStates[static_cast<int>(axis)];

		if (state.value == 0.0f)
			return;

		if (!state.held && !state.pressed)
		{
			state.pressed = true;
			state.held = false;
			state.released = false;

			actionHandler->ProcessBinding(*this, InputControlType::ANALOG, static_cast<int>(axis));
		}
		else
		{
			state.pressed = false;
			state.held = true;
		}
	}

	void MouseInputDevice::HandleMove(const Vector2F& position, const Vector2F& delta)
	{
		axisStates[static_cast<int>(MouseAxisCode::POSITION_X)].value = position.x;
		axisStates[static_cast<int>(MouseAxisCode::POSITION_Y)].value = position.y;

		axisStates[static_cast<int>(MouseAxisCode::MOTION_X)].value += delta.x;
		axisStates[static_cast<int>(MouseAxisCode::MOTION_Y)].value += delta.y;

		UpdateMotionAxisPhase(MouseAxisCode::MOTION_X);
		UpdateMotionAxisPhase(MouseAxisCode::MOTION_Y);
	}

	void MouseInputDevice::HandleWheel(const Vector2F& delta)
	{
		axisStates[static_cast<int>(MouseAxisCode::WHEEL_X)].value += delta.x;
		axisStates[static_cast<int>(MouseAxisCode::WHEEL_Y)].value += delta.y;
	}

	InputControlState MouseInputDevice::GetButtonState(int buttonID) const
	{
		if (buttonID >= 0 && buttonID < (int)buttonStates.size())
			return buttonStates[buttonID];

		return {};
	}

	InputControlState MouseInputDevice::GetAxisState(int axisID) const
	{
		if (axisID >= 0 && axisID < (int)axisStates.size())
			return axisStates[axisID];

		return {};
	}


	InputDeviceType MouseInputDevice::Type() const
	{
		return InputDeviceType::MOUSE;
	}

	InputDeviceID MouseInputDevice::ID() const
	{
		return DefaultDeviceIDs::MOUSE;
	}

	InputControlState MouseInputDevice::GetButtonState(MouseButtonCode code) const
	{
		return GetButtonState(static_cast<int>(code));
	}

	InputControlState MouseInputDevice::GetAxisState(Models::MouseAxisCode code) const
	{
		return GetAxisState(static_cast<int>(code));
	}

	Vector2F MouseInputDevice::GetMousePosition() const
	{
		return Vector2F(
			axisStates[static_cast<int>(MouseAxisCode::POSITION_X)].value,
			axisStates[static_cast<int>(MouseAxisCode::POSITION_Y)].value);
	}

	Vector2F MouseInputDevice::GetMouseDelta() const
	{
		return Vector2F(
			axisStates[static_cast<int>(MouseAxisCode::MOTION_X)].value,
			axisStates[static_cast<int>(MouseAxisCode::MOTION_Y)].value);
	}

	Vector2F MouseInputDevice::GetWheelDelta() const
	{
		return Vector2F(
			axisStates[static_cast<int>(MouseAxisCode::WHEEL_X)].value,
			axisStates[static_cast<int>(MouseAxisCode::WHEEL_Y)].value);
	}
}