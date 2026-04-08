#include "Constants/Input/DefaultDeviceIDs.h"
#include "Constants/Input/DefaultDeviceNames.h"
#include "Converters/Input/MouseButtonConversions.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include <SDL_events.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Internal;

	using namespace Shared::Models;


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

			// released is true only the frame the key was released
			if (state.released)
			{
				state.released = false;

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}
		}

		activeButtonIDs.clear();

		// Axes
		for (auto i = 0; i < static_cast<int>(MouseAxisCode::COUNT_MAX); i++)
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

				actionHandler->ProcessBinding(*this, InputControlType::ANALOG, static_cast<int>(i));
			}
			else
			{
				if (state.held || state.pressed)
				{
					// first frame of stopping
					state.released = true;
					state.pressed = false;
					state.held = false;

					actionHandler->ProcessBinding(*this, InputControlType::ANALOG, static_cast<int>(i));
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

	void MouseInputDevice::ProcessEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
			case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			{
				auto controlID = event.button.button;
				auto& state = buttonStates[controlID];

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

				break;
			}

			case SDL_EventType::SDL_MOUSEBUTTONUP:
			{
				auto controlID = event.button.button;
				auto& state = buttonStates[controlID];

				state.value = 0.0f;
				state.pressed = false;
				state.held = false;
				state.released = true;

				activeButtonIDs.insert(controlID);

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);

				break;
			}

			case SDL_EventType::SDL_MOUSEMOTION:
			{
				auto& motionX = axisStates[static_cast<int>(MouseAxisCode::MOTION_X)];
				auto& motionY = axisStates[static_cast<int>(MouseAxisCode::MOTION_Y)];

				motionX.value += event.motion.xrel;
				motionY.value += event.motion.yrel;

				// pressed/held detection for X
				if (motionX.value != 0.0f)
				{
					if (!motionX.held && !motionX.pressed)
					{
						motionX.pressed = true;
						motionX.held = false;
						motionX.released = false;

						actionHandler->ProcessBinding(*this, InputControlType::ANALOG, static_cast<int>(MouseAxisCode::MOTION_Y));
					}
					else
					{
						motionX.pressed = false;
						motionX.held = true;
					}
				}

				// pressed/held detection for Y
				if (motionY.value != 0.0f)
				{
					if (!motionY.held && !motionY.pressed)
					{
						motionY.pressed = true;
						motionY.held = false;
						motionY.released = false;

						actionHandler->ProcessBinding(*this, InputControlType::ANALOG, static_cast<int>(MouseAxisCode::MOTION_Y));
					}
					else
					{
						motionY.pressed = false;
						motionY.held = true;
					}
				}

				break;
			}

			case SDL_EventType::SDL_MOUSEWHEEL:
			{
				auto& wheelX = axisStates[static_cast<int>(MouseAxisCode::WHEEL_X)];
				auto& wheelY = axisStates[static_cast<int>(MouseAxisCode::WHEEL_Y)];

				wheelX.value += event.wheel.x;
				wheelY.value += event.wheel.y;

				break;
			}
		}
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
		auto sdlCode = static_cast<int>(MouseButtonConversions::ToSDLMouseButton(code));

		return GetButtonState(sdlCode);
	}

	InputControlState MouseInputDevice::GetAxisState(Shared::Models::MouseAxisCode code) const
	{
		return GetAxisState(static_cast<int>(code));
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