#include "Core/SubSystems/Systems/Input/Actions/inputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceManager.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceAddedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceRemovedEvent.h"
#include <SDL.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;

	using namespace Shared::Models;


	DeviceManager::DeviceManager(std::shared_ptr<InputActionResolver> inputActionResolver)
		: inputActionResolver(inputActionResolver)
	{
		// Keyboard and mouse are always initialised and registered
		keyboard = std::make_shared<KeyboardInputDevice>();
		mouse = std::make_shared<MouseInputDevice>();

		otherDevices.emplace(keyboard->ID(), keyboard);
		otherDevices.emplace(mouse->ID(), mouse);


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

		// TODO: This might not be necessary
		// Optionally enumerate existing controllers
		auto n = SDL_NumJoysticks();

		for (auto i = 0; i < n; ++i)
		{
			if (!SDL_IsGameController(i))
				continue;

			OpenController(i);
		}
	}

	DeviceManager::~DeviceManager()
	{
		// Close controller devices
		std::vector<SDL_JoystickID> toClose;
		for (auto& p : otherDevices)
		{
			if (p.first < 0)
				continue;
				
			toClose.push_back(p.first);
		}

		for (auto id : toClose)
		{
			CloseController(id);
		}

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
		auto newDevice = std::make_shared<ControllerInputDevice>(gc, inst);
		
		otherDevices.emplace(inst, newDevice);
		
		EventDispatcher::SendEvent(std::make_shared<DeviceAddedEvent>(newDevice));
	}

	void DeviceManager::CloseController(DeviceID instanceId)
	{
		auto it = otherDevices.find(instanceId);

		if (it == otherDevices.end()) 
			return;

		auto& removedDevice = it->second;

		EventDispatcher::SendEvent(std::make_shared<DeviceRemovedEvent>(removedDevice));

		otherDevices.erase(it);
	}


	void DeviceManager::BeginFrame()
	{
		mouse->BeginFrame(inputActionResolver.get());
		keyboard->BeginFrame(inputActionResolver.get());

		for (auto& d : otherDevices)
		{
			d.second->BeginFrame(inputActionResolver.get());
		}
	}

	void DeviceManager::HandleEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
			// Keyboard
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				keyboard->ProcessEvent(event, inputActionResolver.get());
				break;
			
			// Mouse
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
				mouse->ProcessEvent(event, inputActionResolver.get());
				break;

			// Controller
			case SDL_CONTROLLERDEVICEADDED:
				OpenController(event.cdevice.which);
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				CloseController(event.cdevice.which);
				break;
			
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
			case SDL_CONTROLLERAXISMOTION:
			{
				auto instance_id = (event.type == SDL_CONTROLLERAXISMOTION) ? event.caxis.which : event.cbutton.which;

				auto it = otherDevices.find(instance_id);

				if (it != otherDevices.end())
				{
					it->second->ProcessEvent(event, inputActionResolver.get());
				}

				break;
			}

			default:
				break;
		}
	}

	std::shared_ptr<InputDevice> DeviceManager::GetDevice(DeviceID id)
	{
		auto it = otherDevices.find(id);

		if (it == otherDevices.end()) 
			return nullptr;

		return it->second;
	}

	std::vector<std::shared_ptr<InputDevice>> DeviceManager::GetAllDevices() const
	{
		std::vector<std::shared_ptr<InputDevice>> out;

		// Always include keyboard and mouse
		out.push_back(keyboard);
		out.push_back(mouse);

		// Add all controllers
		for (auto& p : otherDevices)
		{
			out.push_back(p.second);
		}

		return out;
	}

	std::shared_ptr<KeyboardInputDevice> DeviceManager::Keyboard()
	{
		return keyboard;
	}

	std::shared_ptr<MouseInputDevice> DeviceManager::Mouse()
	{
		return mouse;
	}
}