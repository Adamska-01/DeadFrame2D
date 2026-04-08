#include "Constants/Input/DefaultDeviceNames.h"
#include "Constants/Input/InputConstants.h"
#include "Converters/Input/ControllerButtonConversions.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include <SDL_events.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Internal;

	using namespace Shared::Models;


	ControllerInputDevice::ControllerInputDevice(SDL_GameController* controller, InputDeviceID instanceID, IInputActionHandler* actionHandler)
		: InputDevice(SDL_GameControllerName(controller) ? SDL_GameControllerName(controller) : DefaultDeviceNames::CONTROLLER, actionHandler),
		controller(controller), 
		instanceID(instanceID)
	{
		buttonStates = std::vector<DeadFrame2D::Data::InputControlState>(SDL_CONTROLLER_BUTTON_MAX);
		axisStates = std::vector<DeadFrame2D::Data::InputControlState>(SDL_CONTROLLER_AXIS_MAX);
	}

	ControllerInputDevice::~ControllerInputDevice()
	{
		if (controller != nullptr)
		{
			SDL_GameControllerClose(controller);

			controller = nullptr;
		}
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

	void ControllerInputDevice::ProcessEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EventType::SDL_CONTROLLERBUTTONDOWN:
		{
			if (event.cbutton.which != instanceID)
				return;

			auto controlID = event.cbutton.button;
			auto& state = buttonStates[controlID];

			if (!state.held && !state.pressed)
			{
				state.value = 1.0f;
				state.pressed = true;
				state.held = false;
				state.released = false;

				activeButtons.insert(controlID);

				actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);
			}

			break;
		}

		case SDL_EventType::SDL_CONTROLLERBUTTONUP:
		{
			if (event.cbutton.which != instanceID)
				return;

			auto controlID = event.cbutton.button;
			auto& state = buttonStates[controlID];

			state.value = 0.0f;
			state.pressed = false;
			state.held = false;
			state.released = true;

			activeButtons.insert(controlID);

			actionHandler->ProcessBinding(*this, InputControlType::DIGITAL, controlID);

			break;
		}

		case SDL_EventType::SDL_CONTROLLERAXISMOTION:
		{
			if (event.caxis.which != instanceID)
				return;

			auto controlID = event.caxis.axis;

			auto normalizedAxis = std::clamp(
				static_cast<float>(event.caxis.value) / InputConstants::AXIS_MAX_VALUE,
				-1.0f,
				1.0f);

			auto isHeldNow = std::fabs(normalizedAxis) >= InputConstants::ANALOG_PRESS_THRESHOLD;

			auto& state = axisStates[controlID];

			auto wasHeld = state.held;

			state.value = normalizedAxis;
			state.held = isHeldNow;
			state.pressed = !wasHeld && isHeldNow;
			state.released = wasHeld && !isHeldNow;

			activeAxes.insert(controlID);

			actionHandler->ProcessBinding(*this, InputControlType::ANALOG, controlID);

			break;
		}

		default:
			break;
		}
	}

	InputControlState ControllerInputDevice::GetButtonState(int buttonID) const
	{
		if (buttonID >= 0 && buttonID < (int)SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX)
			return buttonStates[buttonID];

		return {};
	}

	InputControlState ControllerInputDevice::GetAxisState(int axisID) const
	{
		if (axisID >= 0 && axisID < (int)SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX)
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
		auto sdlCode = static_cast<int>(ControllerButtonConversions::ToSDLControllerButton(code));

		return GetButtonState(sdlCode);
	}

	InputControlState ControllerInputDevice::GetAxisState(Shared::Models::ControllerAxisCode code) const
	{
		auto sdlCode = static_cast<int>(ControllerButtonConversions::ToSDLControllerAxis(code));

		return GetAxisState(sdlCode);
	}
}