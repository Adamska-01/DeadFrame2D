#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Input/DeviceAddedEvent.h"
#include "Engine/Events/Context/Input/DeviceRemovedEvent.h"
#include "Utilities/Debugging/Guards.h"
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	DeviceManager::DeviceManager(IInputActionHandler* actionHandler, std::function<void(InputDeviceID)> onDeviceRemoved)
		: onDeviceRemoved(std::move(onDeviceRemoved))
	{
		this->actionHandler = Guard::AgainstNullAssignment(actionHandler, NAME_OF(actionHandler));

		// Keyboard and mouse are always initialised and registered
		auto keyboardDevice = std::make_unique<KeyboardInputDevice>(actionHandler);
		auto mouseDevice = std::make_unique<MouseInputDevice>(actionHandler);

		keyboard = keyboardDevice.get();
		mouse = mouseDevice.get();

		devices[DefaultDeviceIDs::KEYBOARD] = std::move(keyboardDevice);
		devices[DefaultDeviceIDs::MOUSE] = std::move(mouseDevice);
	}

	DeviceManager::~DeviceManager()
	{
	}


	void DeviceManager::OnSystemEvent(const SystemEvent& systemEvent)
	{
		std::visit(
			[&](const auto& event)
			{
				using T = std::decay_t<decltype(event)>;

				if constexpr (std::is_same_v<T, KeyEvent>)
				{
					static_cast<InputDevice*>(keyboard)->HandleEvent(systemEvent);
				}
				else if constexpr (std::is_same_v<T, MouseButtonEvent>
					|| std::is_same_v<T, MouseMoveEvent>
					|| std::is_same_v < T, MouseWheelEvent>)
				{
					static_cast<InputDevice*>(mouse)->HandleEvent(systemEvent);
				}
				else if constexpr (std::is_same_v<T, ControllerConnectedEvent>)
				{
					AddController(event.deviceID, event.name);
				}
				else if constexpr (std::is_same_v<T, ControllerDisconnectedEvent>)
				{
					RemoveController(event.deviceID);
				}
				else if constexpr (std::is_same_v<T, ControllerButtonEvent> || std::is_same_v<T, ControllerAxisEvent>)
				{
					if (auto* controller = Controller(event.deviceID))
					{
						static_cast<InputDevice*>(controller)->HandleEvent(systemEvent);

						currentController = controller;
					}
				}
			},
			systemEvent);
	}

	void DeviceManager::AddController(InputDeviceID instanceID, const std::string& name)
	{
		auto newDevice = std::make_unique<ControllerInputDevice>(instanceID, name, actionHandler);

		auto* devicePtr = newDevice.get();

		devices[instanceID] = std::move(newDevice);

		if (currentController == nullptr)
		{
			currentController = devicePtr;
		}

		std::cout << "[Input] Device added: " << devicePtr->Name() << " (ID: " << devicePtr->ID() << ")" << std::endl;

		EventDispatcher::SendEvent(std::make_shared<DeviceAddedEvent>(devicePtr->ID(), devicePtr->Name()));
	}

	void DeviceManager::RemoveController(InputDeviceID instanceID)
	{
		auto it = devices.find(instanceID);

		if (it == devices.end())
			return;

		auto& removedDevice = it->second;

		if (currentController != nullptr && currentController->ID() == removedDevice->ID())
		{
			currentController = nullptr;
		}

		if (onDeviceRemoved)
		{
			onDeviceRemoved(instanceID);
		}

		std::cout << "[Input] Device removed: " << removedDevice->Name() << " (ID: " << removedDevice->ID() << ")" << std::endl;

		EventDispatcher::SendEvent(std::make_shared<DeviceRemovedEvent>(removedDevice->ID(), removedDevice->Name()));

		devices.erase(it);
	}

	void DeviceManager::BeginFrame()
	{
		for (auto& [id, device] : devices)
		{
			device->BeginFrame();
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

		out.reserve(devices.size());

		for (auto& [id, device] : devices)
		{
			out.push_back(device.get());
		}

		return out;
	}

	KeyboardInputDevice* DeviceManager::Keyboard()
	{
		return keyboard;
	}

	MouseInputDevice* DeviceManager::Mouse()
	{
		return mouse;
	}

	ControllerInputDevice* DeviceManager::Controller(Data::InputDeviceID id)
	{
		if (id == DefaultDeviceIDs::KEYBOARD || id == DefaultDeviceIDs::MOUSE)
			return nullptr;

		auto it = devices.find(id);

		return (it != devices.end()) ? static_cast<ControllerInputDevice*>(it->second.get()) : nullptr;
	}

	ControllerInputDevice* DeviceManager::CurrentController()
	{
		return static_cast<ControllerInputDevice*>(currentController);
	}
}