#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
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

	DeviceID KeyboardInputDevice::ID() const
	{
		return DefaultDeviceIDs::KEYBOARD;
	}

	void KeyboardInputDevice::BeginFrame()
	{
		for (auto& keyCode : activeKeys)
		{
			auto& state = states[keyCode];

			// pressed is true only the first frame, then becomes held
			if (state.pressed)
			{
				state.pressed = false;
				state.held = true;
			}

			// released is true only the frame the key was released
			if (state.released)
			{
				state.released = false;
			}
		}

		activeKeys.clear();
	}

	int KeyboardInputDevice::ProcessEvent(const SDL_Event& event)
	{
		auto eventType = event.type;

		switch (eventType)
		{
			case SDL_EventType::SDL_KEYDOWN:
			{
				auto keyCode = event.key.keysym.scancode;
				auto& state = states[keyCode];

				// Only set pressed if it wasn't already held
				if (!state.held && !state.pressed)
				{
					state.value = 1.0f;
					state.pressed = true;
					state.held = false;
					state.released = false;

					activeKeys.insert(keyCode);
				}

				break;
			}
			case SDL_EventType::SDL_KEYUP:
			{
				auto keyCode = event.key.keysym.scancode;
				auto& state = states[keyCode];

				state.value = 0.0f;
				state.pressed = false;
				state.held = false;
				state.released = true;

				activeKeys.insert(keyCode);

				break;
			}

			default:
				break;
		}

		return -1;
	}

	InputControlState KeyboardInputDevice::GetKeyState(int controlID) const
	{
		if (controlID < 0 || controlID >= SDL_Scancode::SDL_NUM_SCANCODES)
			return {};

		return states[controlID];
	}
}