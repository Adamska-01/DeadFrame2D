#include "SubSystems/Input/Devices/KeyboardInputDevice.h"
#include <SDL.h>


KeyboardInputDevice::KeyboardInputDevice()
{
}

void KeyboardInputDevice::ProcessEvent(const SDL_Event& eventType)
{
	lastInputStates = currentInputStates;
	lastMouseInputStates = currentMouseInputStates;

	switch (eventType.type)
	{
	case SDL_EventType::SDL_KEYDOWN:
		currentInputStates[eventType.key.keysym.scancode] = true;
		break;

	case SDL_EventType::SDL_KEYUP:
		currentInputStates[eventType.key.keysym.scancode] = false;
		break;

	default:
		break;
	}
}