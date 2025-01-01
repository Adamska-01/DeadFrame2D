#include "SubSystems/Events/EventManager.h"
#include "SubSystems/Input.h"
#include <iostream>


std::unordered_map<InputDeviceFlag, InputState> Input::currentInputs = {};

std::unordered_map<InputDeviceFlag, InputState> Input::lastInputs = {};

std::vector<SDL_GameController*> Input::controllers = {};


Input::Input()
{
	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0)
	{
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	}

	EventManager::AddEventProcessor(this);
}

Input::~Input()
{
	Cleanup();
}

void Input::InitialiseControllers()
{
	for (auto i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (!SDL_IsGameController(i))
			continue;

		auto pad = SDL_GameControllerOpen(i);
		if (pad == nullptr)
			continue;

		controllers.push_back(pad);
	}
}

void Input::ProcessControllerEvents(const SDL_Event& controllerEvent)
{
	if (controllers.size() > controllerEvent.cdevice.which)
		return;

	auto deviceID = static_cast<InputDeviceFlag>(InputDeviceFlag::DEVICE_CONTROLLER_1 << controllerEvent.cdevice.which);
	auto& state = currentInputs[deviceID];

	switch (controllerEvent.type)
	{
	case SDL_CONTROLLERBUTTONDOWN:
		state.buttons[controllerEvent.cbutton.button] = true;
		break;

	case SDL_CONTROLLERBUTTONUP:
		state.buttons[controllerEvent.cbutton.button] = false;
		break;

	case SDL_CONTROLLERAXISMOTION:
		state.axes[controllerEvent.caxis.axis] = controllerEvent.caxis.value / 32768.0f;
		break;

	default:
		break;
	}
}

void Input::ProcessKeyboardEvents(const SDL_Event& keyboardEvent)
{
	auto& state = currentInputs[InputDeviceFlag::DEVICE_KEYBOARD];

	switch (keyboardEvent.type)
	{
	case SDL_KEYDOWN:
		state.buttons[keyboardEvent.key.keysym.scancode] = true;
		break;

	case SDL_KEYUP:
		state.buttons[keyboardEvent.key.keysym.scancode] = false;
		break;

	default:
		break;
	}
}

void Input::ProcessMouseEvents(const SDL_Event& mouseEvent)
{
	auto& state = currentInputs[InputDeviceFlag::DEVICE_MOUSE];

	switch (mouseEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		state.buttons[mouseEvent.button.button] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		state.buttons[mouseEvent.button.button] = false;
		break;

	case SDL_MOUSEMOTION:
		state.axes[SDL_CONTROLLER_AXIS_LEFTX] = mouseEvent.motion.xrel;
		state.axes[SDL_CONTROLLER_AXIS_LEFTY] = mouseEvent.motion.yrel;
		break;

	default:
		break;
	}
}

bool Input::IsDeviceValid(InputDeviceFlag deviceFlag)
{
	return currentInputs.find(deviceFlag) != currentInputs.end();
}

void Input::Cleanup()
{
	for (auto controller : controllers)
	{
		SDL_GameControllerClose(controller);
	}

	controllers.clear();

	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

std::optional<int> Input::ProcessEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
	case SDL_CONTROLLERAXISMOTION:
		lastInputs = currentInputs;
		ProcessControllerEvents(sdlEvent);
		break;

	case SDL_KEYDOWN:
	case SDL_KEYUP:
		lastInputs = currentInputs;
		ProcessKeyboardEvents(sdlEvent);
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEMOTION:
		lastInputs = currentInputs;
		ProcessMouseEvents(sdlEvent);
		break;

	case SDL_CONTROLLERDEVICEADDED:
	case SDL_CONTROLLERDEVICEREMOVED:
		Cleanup();
		InitialiseControllers();
		break;
	}

	return std::nullopt;
}

bool Input::IsButtonPressed(InputDeviceFlag deviceFlag, int button)
{
	if (!IsDeviceValid(deviceFlag))
		return false;

	return currentInputs[deviceFlag].buttons[button] && !lastInputs[deviceFlag].buttons[button];
}

bool Input::IsButtonHeld(InputDeviceFlag deviceFlag, int button)
{
	if (!IsDeviceValid(deviceFlag))
		return false;

	return currentInputs[deviceFlag].buttons[button];
}

float Input::GetAxisValue(InputDeviceFlag deviceFlag, int axis)
{
	if (!IsDeviceValid(deviceFlag))
		return 0.0f;

	return currentInputs[deviceFlag].axes[axis];
}

bool Input::IsControllerConnected(InputDeviceFlag deviceFlag)
{
	if (deviceFlag < InputDeviceFlag::DEVICE_CONTROLLER_1 || deviceFlag > InputDeviceFlag::DEVICE_CONTROLLER_4)
	{
		std::cerr << "Invalid device flag: " << deviceFlag << std::endl;
		return false;
	}

	// Calculate the controller index based on the flag (Normalize)
	auto controllerIndex = static_cast<int>(log2(deviceFlag >> 2));

	return controllerIndex >= 0 && controllerIndex < controllers.size() && SDL_GameControllerGetAttached(controllers[controllerIndex]);
}