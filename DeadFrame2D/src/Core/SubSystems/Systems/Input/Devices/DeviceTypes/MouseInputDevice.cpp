#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
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

	DeviceID MouseInputDevice::ID() const
	{
		return DefaultDeviceIDs::MOUSE;
	}

	void MouseInputDevice::BeginFrame()
	{
		for (auto& keyCode : activeButtons)
		{
			auto& state = buttonStates[keyCode];

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

		activeButtons.clear();


		mouseDelta.x = 0.0f;
		mouseDelta.y = 0.0f;

		wheelDelta.x = 0.0f;
		wheelDelta.y = 0.0f;

		wasMotionThisFrame = false;
	}

	int MouseInputDevice::ProcessEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
			case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			{
				auto buttonCode = event.button.button;
				auto& state = buttonStates[buttonCode];

				// Only set pressed if it wasn't already held
				if (!state.held && !state.pressed)
				{
					state.value = 1.0f;
					state.pressed = true;
					state.held = false;
					state.released = false;

					activeButtons.insert(buttonCode);
				}

				break;
			}

			case SDL_EventType::SDL_MOUSEBUTTONUP:
			{
				auto buttonCode = event.button.button;
				auto& state = buttonStates[buttonCode];

				state.value = 0.0f;
				state.pressed = false;
				state.held = false;
				state.released = true;

				activeButtons.insert(buttonCode);

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

		return -1;
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