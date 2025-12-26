#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Data/Input/DefaultDeviceIDs.h"
#include "Data/Input/DefaultDeviceNames.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;
	using namespace Shared::Models;


	MouseInputDevice::MouseInputDevice()
		: InputDevice(DefaultDeviceNames::MOUSE)
	{
		buttonStates.fill({});
	}

	InputDeviceType MouseInputDevice::Type() const
	{
		return InputDeviceType::MOUSE;
	}

	InputDeviceID MouseInputDevice::ID() const
	{
		return DefaultDeviceIDs::MOUSE;
	}

	void MouseInputDevice::BeginFrame()
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());

		for (auto& controlID : activeControlIDs)
		{
			auto& state = buttonStates[controlID];

			// pressed is true only the first frame, then becomes held
			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;

				actionsFrameManagement->ProcessBinding(*this, controlID);
			}

			// released is true only the frame the key was released
			if (state.released)
			{
				state.released = false;

				actionsFrameManagement->ProcessBinding(*this, controlID);
			}
		}

		activeControlIDs.clear();


		mouseDelta.x = 0.0f;
		mouseDelta.y = 0.0f;

		wheelDelta.x = 0.0f;
		wheelDelta.y = 0.0f;

		wasMotionThisFrame = false;
	}

	void MouseInputDevice::ProcessEvent(const SDL_Event& event)
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());

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

					activeControlIDs.insert(controlID);

					actionsFrameManagement->ProcessBinding(*this, controlID);
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

				activeControlIDs.insert(controlID);

				actionsFrameManagement->ProcessBinding(*this, controlID);
				
				break;
			}

			case SDL_EventType::SDL_MOUSEMOTION:
				mouseDelta.x += event.motion.xrel;
				mouseDelta.y += event.motion.yrel;

				wasMotionThisFrame = true;

				break;

			case SDL_EventType::SDL_MOUSEWHEEL:
				wheelDelta.x += (float)event.wheel.x;
				wheelDelta.y += (float)event.wheel.y;

				break;
		}
	}

	InputControlState MouseInputDevice::GetKeyState(int controlId) const
	{
		if (controlId >= 1 && controlId < (int)buttonStates.size()) 
			return buttonStates[controlId];

		return {};
	}

	Vector2F MouseInputDevice::GetMouseDelta() const
	{
		return mouseDelta;
	}

	Vector2F MouseInputDevice::GetWheelDelta() const
	{
		return wheelDelta;
	}
}