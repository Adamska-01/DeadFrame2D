#include "SubSystems/Input/Devices/MouseInputDevice.h"


void MouseInputDevice::ProcessEvent(const SDL_Event& eventType)
{
	lastInputStates = currentInputStates;
	
	switch (eventType.type)
	{
	case SDL_EventType::SDL_MOUSEBUTTONDOWN:
		currentInputStates[eventType.button.button] = true;
		break;

	case SDL_EventType::SDL_MOUSEBUTTONUP:
		currentInputStates[eventType.button.button] = false;
		break;

		// TODO: Add axis support
	case SDL_EventType::SDL_MOUSEMOTION:
		break;

	default:
		break;
	}
}