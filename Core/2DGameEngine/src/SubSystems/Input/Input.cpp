#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/Input/ControllerDisconnectedEvent.h"
#include "SubSystems/Input/Devices/ControllerInputDevice.h"
#include "SubSystems/Input/Devices/KeyboardInputDevice.h"
#include "SubSystems/Input/Devices/MouseInputDevice.h"
#include "SubSystems/Input/Input.h"
#include "SubSystems/Input/InputControls.h"
#include "Tools/Helpers/EventHelpers.h"
#include <Constants/SharedResourcePaths.h>
#include <iostream>
#include <unordered_set>


std::unique_ptr<InputDevice> Input::keyboardDevice;

std::unique_ptr<InputDevice> Input::mouseDevice;

std::vector<std::unique_ptr<InputDevice>> Input::controllerDevices = {};


Input::Input()
{
	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0)
	{
		std::cout << "[Info] SDL_GAMECONTROLLER subsystem already initialized." << std::endl;

		return;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::cerr << "[Error] Failed to initialize SDL_GAMECONTROLLER subsystem: " << SDL_GetError() << std::endl;
			
		return;
	}

	std::cout << "[Info] SDL_GAMECONTROLLER subsystem successfully initialized." << std::endl;
	
	// Disable correlation between RawInput and XInput for Xbox controllers. This prevents 
	// SDL from detecting the same physical Xbox controller twice, once through RawInput 
	// and once through XInput, which can lead to duplicate devices being detected.
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "0");

	InputControls::Deserialize(SharedResourcePaths::Configurations::INPUT_CONTROLS_CONFIGURATION_PATH);

	// Initialize default devices
	keyboardDevice = std::make_unique<KeyboardInputDevice>();
	mouseDevice = std::make_unique<MouseInputDevice>();

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(ControllerDisconnectedEvent)), EventHelpers::BindFunction(this, &Input::DisconnectControllerHandler));
}

Input::~Input()
{
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(ControllerDisconnectedEvent)), EventHelpers::BindFunction(this, &Input::DisconnectControllerHandler));
	
	controllerDevices.clear();

	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0) 
	{
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	
		std::cout << "[Info] SDL_GAMECONTROLLER subsystem successfully shut down." << std::endl;
	}
	else 
	{
		std::cout << "[Info] SDL_GAMECONTROLLER subsystem was not initialized, no need to shut down." << std::endl;
	}
}

void Input::BeginFrame()
{
	for (const auto& device : controllerDevices)
	{
		device->BeginInputFrame();
	}
	keyboardDevice->BeginInputFrame();
	mouseDevice->BeginInputFrame();
}

void Input::DisconnectControllerHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto controllerDisconnectedEvent = DispatchableEvent::SafeCast<ControllerDisconnectedEvent>(dispatchableEvent);

	if (controllerDisconnectedEvent == nullptr || controllerDisconnectedEvent->controllerDevice == nullptr)
		return;

	auto target = controllerDisconnectedEvent->controllerDevice;

	auto it = std::remove_if(
		controllerDevices.begin(),
		controllerDevices.end(),
		[target](const std::unique_ptr<InputDevice>& device)
		{
			return device.get() == target;
		});

	if (it == controllerDevices.end())
		return;

	controllerDevices.erase(it, controllerDevices.end());
}

std::optional<int> Input::ProcessEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_EventType::SDL_CONTROLLERBUTTONDOWN:
	case SDL_EventType::SDL_CONTROLLERBUTTONUP:
	case SDL_EventType::SDL_CONTROLLERAXISMOTION:
	case SDL_EventType::SDL_CONTROLLERDEVICEREMOVED:
		for (const auto& device : controllerDevices)
		{
			device->ProcessEvent(sdlEvent);
		}
		break;

	case SDL_EventType::SDL_KEYDOWN:
	case SDL_EventType::SDL_KEYUP:
		keyboardDevice->ProcessEvent(sdlEvent);
		break;

	case SDL_EventType::SDL_MOUSEBUTTONDOWN:
	case SDL_EventType::SDL_MOUSEBUTTONUP:
	case SDL_EventType::SDL_MOUSEMOTION:
		mouseDevice->ProcessEvent(sdlEvent);
		break;
		
	case SDL_EventType::SDL_CONTROLLERDEVICEADDED:
		// THIS LOGIC IS TEMPORARY
		auto newController = new ControllerInputDevice((int)sdlEvent.cdevice.which);

		auto playerAssigned = PlayerInputSlot::NONE;
		std::unordered_set<PlayerInputSlot> assignedSlots;

		for (const auto& controller : controllerDevices)
		{
			assignedSlots.insert(controller->GetAssignedPlayer());
		}

		// Find first unassigned slot
		for (int i = (int)PlayerInputSlot::PLAYER_1; i <= (int)PlayerInputSlot::PLAYER_4; i++)
		{
			auto currentSlot = static_cast<PlayerInputSlot>(i);
			
			if (assignedSlots.contains(currentSlot))
				continue;

			playerAssigned = currentSlot;

			break;
		}

		newController->AssignedPlayer(playerAssigned);

		controllerDevices.push_back(std::unique_ptr<ControllerInputDevice>(std::move(newController)));
		break;
	}

	return std::nullopt;
}

bool Input::IsButtonPressed(PlayerInputSlot playerSlot, const char* actionName)
{
	auto controlAction = InputControls::GetAction(actionName);

	if (controlAction == std::nullopt)
		return false;

	for (const auto& action : controlAction.value())
	{
		switch (action.inputDeviceType)
		{
		case InputDeviceType::KEYBOARD:
			if (playerSlot == PlayerInputSlot::PLAYER_1 && keyboardDevice->IsKeyPressed(action.inputKey))
				return true;
			break;

		case InputDeviceType::MOUSE:
			if (playerSlot == PlayerInputSlot::PLAYER_1 && mouseDevice->IsKeyPressed(action.inputKey))
				return true;
			break;

		case InputDeviceType::CONTROLLER:
			auto controller = std::find_if(
				controllerDevices.begin(),
				controllerDevices.end(),
				[playerSlot, &action](const auto& controller)
				{
					return controller->GetAssignedPlayer() == playerSlot && controller->IsKeyPressed(action.inputKey);
				});

			if (controller != controllerDevices.end())
				return true;
			break;
		}
	}

	return false;
}

bool Input::IsButtonHeld(PlayerInputSlot playerSlot, const char* actionName)
{
	auto controlAction = InputControls::GetAction(actionName);

	if (controlAction == std::nullopt)
		return false;

	for (const auto& action : controlAction.value())
	{
		switch (action.inputDeviceType)
		{
		case InputDeviceType::KEYBOARD:
			if (playerSlot == PlayerInputSlot::PLAYER_1 && keyboardDevice->IsKeyHeld(action.inputKey))
				return true;
			break;

		case InputDeviceType::MOUSE:
			if (playerSlot == PlayerInputSlot::PLAYER_1 && mouseDevice->IsKeyHeld(action.inputKey))
				return true;
			break;

		case InputDeviceType::CONTROLLER:
			auto controller = std::find_if(
				controllerDevices.begin(),
				controllerDevices.end(),
				[playerSlot, &action](const auto& controller)
				{
					return controller->GetAssignedPlayer() == playerSlot && controller->IsKeyHeld(action.inputKey);
				});

			if (controller != controllerDevices.end())
				return true;
			break;
		}
	}

	return false;
}

// TODO: Implement input axis
float Input::GetAxisValue(PlayerInputSlot playerSlot, const char* actionName)
{
	return 0.0f;
}