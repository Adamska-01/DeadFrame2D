#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceManager.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceAddedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceRemovedEvent.h"
#include "Utilities/Debugging/Guards.h"
#include <SDL.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Utilities;
	using namespace DeadFrame2D::Models;


	DeviceManager::DeviceManager(IInputActionHandler* actionHandler)
		: currentController(nullptr)
	{
		this->actionHandler = Guard::AgainstNullAssignment(actionHandler, NAME_OF(actionHandler));
		
		// Keyboard and mouse are always initialised and registered
		devices[DefaultDeviceIDs::KEYBOARD] = std::make_shared<KeyboardInputDevice>(actionHandler);
		devices[DefaultDeviceIDs::MOUSE] = std::make_shared<MouseInputDevice>(actionHandler);


		// IMPORTANT: If you want to disable RawInput/XInput correlation, set the hint
		// BEFORE initializing the controller subsystem.
		SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "0");

		if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0)
		{
			if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
			{
				std::cerr << "[Input] Failed to init SDL_GAMECONTROLLER: " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "[Info] Input subsystem successfully initialized." << std::endl;
			}
		}
	}

	DeviceManager::~DeviceManager()
	{
		// Close controller devices
		std::vector<InputDeviceID> toClose;
		for (auto& [id, device] : devices)
		{
			if (id == DefaultDeviceIDs::KEYBOARD || id == DefaultDeviceIDs::MOUSE)
				continue;

			toClose.push_back(id);
		}

		for (auto id : toClose)
		{
			CloseController(id);
		}

		devices.clear();

		if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0)
		{
			SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

			std::cout << "[Info] Input subsystem successfully shut down." << std::endl;
		}
		else
		{
			std::cout << "[Info] Input subsystem was not initialized, no need to shut down." << std::endl;
		}
	}


	void DeviceManager::OpenController(int deviceIndex)
	{
		if (!SDL_IsGameController(deviceIndex))
			return;

		auto gc = SDL_GameControllerOpen(deviceIndex);
		
		if (!gc)
		{
			std::cerr << "[Input] Failed open controller index " << deviceIndex << ": " << SDL_GetError() << std::endl;
			
			return;
		}
		
		auto joystick = SDL_GameControllerGetJoystick(gc);
		auto inst = SDL_JoystickInstanceID(joystick);
		auto newDevice = std::make_shared<ControllerInputDevice>(gc, inst, actionHandler);
		
		devices.emplace(inst, newDevice);

		if (currentController == nullptr)
		{
			currentController = newDevice.get();
		}
		
		EventDispatcher::SendEvent(std::make_shared<DeviceAddedEvent>(newDevice->ID(), newDevice->Name()));
	}

	void DeviceManager::CloseController(InputDeviceID instanceId)
	{
		auto it = devices.find(instanceId);

		if (it == devices.end()) 
			return;

		auto& removedDevice = it->second;

		if (currentController != nullptr && currentController->ID() == removedDevice->ID())
		{
			currentController = nullptr;
		}

		EventDispatcher::SendEvent(std::make_shared<DeviceRemovedEvent>(removedDevice->ID(), removedDevice->Name()));

		devices.erase(it);
	}

	std::optional<int> DeviceManager::ProcessEvents(const SDL_Event& sdlEvent)
	{
		switch (sdlEvent.type)
		{
		// Keyboard
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			devices[DefaultDeviceIDs::KEYBOARD]->ProcessEvent(sdlEvent);
			break;

		// Mouse
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			devices[DefaultDeviceIDs::MOUSE]->ProcessEvent(sdlEvent);
			break;

		// Controller
		case SDL_CONTROLLERDEVICEADDED:
			OpenController(sdlEvent.cdevice.which);
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			CloseController(sdlEvent.cdevice.which);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERAXISMOTION:
		{
			auto instance_id = (sdlEvent.type == SDL_CONTROLLERAXISMOTION) ? sdlEvent.caxis.which : sdlEvent.cbutton.which;

			auto it = devices.find(instance_id);

			if (it != devices.end())
			{
				it->second->ProcessEvent(sdlEvent);

				currentController = it->second.get();
			}

			break;
		}

		default:
			break;
		}

		return std::nullopt;
	}

	void DeviceManager::BeginFrame()
	{
		for (auto& d : devices)
		{
			d.second->BeginFrame();
		}
	}

	void DeviceManager::PreUpdate()
	{
	}

	InputDevice* DeviceManager::GetDevice(InputDeviceID id)
	{
		auto it = devices.find(id);

		return (it != devices.end()) ? it->second.get() : nullptr;
	}

	std::vector<InputDevice*> DeviceManager::GetAllDevices() const
	{
		std::vector<InputDevice*> out;

		// Add all controllers
		for (auto& [id, device] : devices)
		{
			out.push_back(device.get());
		}

		return out;
	}

	KeyboardInputDevice* DeviceManager::Keyboard()
	{
		return static_cast<KeyboardInputDevice*>(devices[DefaultDeviceIDs::KEYBOARD].get());
	}

	MouseInputDevice* DeviceManager::Mouse()
	{
		return static_cast<MouseInputDevice*>(devices[DefaultDeviceIDs::MOUSE].get());
	}

	ControllerInputDevice* DeviceManager::Controller(Data::InputDeviceID id)
	{
		auto it = devices.find(id);

		return (it != devices.end()) ? static_cast<ControllerInputDevice*>(it->second.get()) : nullptr;
	}

	ControllerInputDevice* DeviceManager::CurrentController()
	{
		return static_cast<ControllerInputDevice*>(currentController);
	}
}