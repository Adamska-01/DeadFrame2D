#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Data/Input/DefaultDeviceIDs.h"
#include "Data/Input/DefaultDeviceNames.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;
	using namespace Shared::Models;


	KeyboardInputDevice::KeyboardInputDevice()
		: InputDevice(DefaultDeviceNames::KEYBOARD)
	{
		states.fill({});
	}

	InputDeviceType KeyboardInputDevice::Type() const
	{
		return InputDeviceType::KEYBOARD;
	}

	InputDeviceID KeyboardInputDevice::ID() const
	{
		return DefaultDeviceIDs::KEYBOARD;
	}

	void KeyboardInputDevice::BeginFrame()
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());

		for (auto& controlID : activeControlIDs)
		{
			auto& state = states[controlID];

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
	}

	void KeyboardInputDevice::ProcessEvent(const SDL_Event& event)
	{
		auto actionsFrameManagement = static_cast<IInputActionsFrameManagement*>(Input::Actions());
		
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

					actionsFrameManagement->ProcessBinding(*this, controlID);
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
				
				actionsFrameManagement->ProcessBinding(*this, controlID);

				break;
			}

			default:
				break;
		}
	}

	InputControlState KeyboardInputDevice::GetKeyState(int controlID) const
	{
		if (controlID < 0 || controlID >= SDL_Scancode::SDL_NUM_SCANCODES)
			return {};

		return states[controlID];
	}
}