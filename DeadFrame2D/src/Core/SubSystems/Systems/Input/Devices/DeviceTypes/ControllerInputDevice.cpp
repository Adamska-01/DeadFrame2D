#include "Constants/Input/InputConstants.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionsFrameManagement.h"
#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Data/Input/DefaultDeviceNames.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;

	using namespace Shared::Models;


	ControllerInputDevice::ControllerInputDevice(SDL_GameController* controller, InputDeviceID instanceID)
		: InputDevice(SDL_GameControllerName(controller) ? SDL_GameControllerName(controller) : DefaultDeviceNames::CONTROLLER),
		controller(controller), 
		instanceID(instanceID)
	{
	}

	ControllerInputDevice::~ControllerInputDevice()
	{
		if (controller != nullptr)
		{
			SDL_GameControllerClose(controller);
			
			controller = nullptr;
		}
	}


	InputDeviceType ControllerInputDevice::Type() const
	{
		return InputDeviceType::CONTROLLER;
	}

	InputDeviceID ControllerInputDevice::ID() const
	{
		return instanceID;
	}


	int ControllerInputDevice::NormalizedAxisID(int axisID) const
	{
		// Map SDL axes to integers
		return axisID - (int)SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX; // Axes start from 200
	}


	void ControllerInputDevice::BeginFrame()
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());

		for (auto button : activeButtons)
		{
			auto& state = buttonStates[button];

			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionsFrameManagement->ProcessBinding(*this, button);
			}
			
			if (state.released)
			{
				state.released = false;
				state.value = 0.0f;

				actionsFrameManagement->ProcessBinding(*this, button);
			}
		}

		for (auto axis : activeAxes)
		{
			auto& state = axisStates[axis];

			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionsFrameManagement->ProcessBinding(*this, axis);
			}

			if (state.released)
			{
				state.released = false;
				state.value = 0.0f;

				actionsFrameManagement->ProcessBinding(*this, axis);
			}
		}

		activeButtons.clear();
		activeAxes.clear();
	}

	void ControllerInputDevice::ProcessEvent(const SDL_Event& event)
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());

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

					actionsFrameManagement->ProcessBinding(*this, controlID);
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

				actionsFrameManagement->ProcessBinding(*this, controlID);

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

				actionsFrameManagement->ProcessBinding(*this, controlID);

				break;
			}

			default:
				break;
		}
	}

	InputControlState ControllerInputDevice::GetKeyState(int controlID) const
	{
		if (controlID >= 0 && controlID < (int)SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX)
			return buttonStates[controlID];
		else if (controlID >= (int)SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX && controlID < ((int)SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX + (int)SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX))
			return axisStates[NormalizedAxisID(controlID)];

		return {};
	}
}