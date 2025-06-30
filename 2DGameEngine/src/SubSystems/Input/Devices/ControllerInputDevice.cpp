#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/Input/ControllerDisconnectedEvent.h"
#include "SubSystems/Input/Devices/ControllerInputDevice.h"
#include "SubSystems/Input/InputControls.h"
#include <iostream>
#include <stdexcept>
#include <string>


ControllerInputDevice::ControllerInputDevice(int deviceID)
{
	if (!SDL_IsGameController(deviceID))
		throw std::invalid_argument("The device (ID: " + std::to_string(deviceID) + ") is not a valid game controller.");

	this->deviceID = deviceID;

	gameControllerDevice = SDL_GameControllerOpen(deviceID);
	
	if (gameControllerDevice == nullptr)
		throw std::runtime_error("Failed to open game controller: " + std::string(SDL_GetError()));

	std::cout << "Connected Game Controller " << deviceID << ": " << SDL_JoystickNameForIndex(deviceID) << std::endl;

	for (const auto& [actionName, actionMappings] : InputControls::GetAllActions())
	{
		for (const auto& mapping : actionMappings)
		{
			if (mapping.inputDeviceType != InputDeviceType::CONTROLLER)
				continue;

			// Initialize the key state to false (not pressed)
			currentInputStates.insert({ mapping.inputKey, false });
		}
	}
}

ControllerInputDevice::~ControllerInputDevice()
{
	SDL_GameControllerClose(gameControllerDevice);

	gameControllerDevice = nullptr;
}

void ControllerInputDevice::ProcessEvent(const SDL_Event& controllerEvent)
{
	if (deviceID != controllerEvent.cdevice.which)
		return;

	switch (controllerEvent.type)
	{
	case SDL_EventType::SDL_CONTROLLERBUTTONDOWN:
		currentInputStates[controllerEvent.cbutton.button] = true;
		break;

	case SDL_EventType::SDL_CONTROLLERBUTTONUP:
		currentInputStates[controllerEvent.cbutton.button] = false;
		break;

		// TODO: Add axis support
	case SDL_EventType::SDL_CONTROLLERAXISMOTION:
		break;

	case SDL_EventType::SDL_CONTROLLERDEVICEREMOVED:
		EventDispatcher::SendEvent(std::make_shared<ControllerDisconnectedEvent>(this));
		break;

	default:
		break;
	}
}