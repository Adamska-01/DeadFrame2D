#include "SubSystems/Input/Devices/MouseInputDevice.h"
#include <SDL.h>


MouseInputDevice::MouseInputDevice()
{
}

void MouseInputDevice::ProcessEvent(const SDL_Event& eventType)
{
	lastInputStates = currentInputStates;
	lastMouseInputStates = currentMouseInputStates;
	
	switch (eventType.type)
	{
	case SDL_EventType::SDL_MOUSEBUTTONDOWN:
		currentMouseInputStates[eventType.button.button] = true;
		break;

	case SDL_EventType::SDL_MOUSEBUTTONUP:
		currentMouseInputStates[eventType.button.button] = false;
		break;

		// TODO: Add axis support
	case SDL_EventType::SDL_MOUSEMOTION:
		break;

	default:
		break;
	}
}
