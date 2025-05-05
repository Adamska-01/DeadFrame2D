#include "SubSystems/Input/Devices/KeyboardInputDevice.h"
#include "SubSystems/Input/InputControls.h"


KeyboardInputDevice::KeyboardInputDevice()
{
	for (const auto& [actionName, actionMappings] : InputControls::GetAllActions())
	{
		for (const auto& mapping : actionMappings)
		{
			if (mapping.inputDeviceType != InputDeviceType::KEYBOARD)
				continue;

			// Initialize the key state to false (not pressed)
			currentInputStates.insert({ mapping.inputKey, false });
		}
	}
}

void KeyboardInputDevice::ProcessEvent(const SDL_Event& eventType)
{
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