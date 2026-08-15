#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/Context/Systems/Input/Devices/Abstractions/IInputDeviceProvider.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "Data/Systems/Events/SystemEvent.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>


namespace DF2D::Engine
{
	class EventDispatcher;
}


namespace DF2D::Core
{
	class IInputActionHandler;
	class KeyboardInputDevice;
	class MouseInputDevice;


	/**
	 * @brief Routes engine input events to connected devices.
	 */
	class DF2D_API DeviceManager : public IInputDeviceProvider, public ISystemEventSink, public IInputFrameLifecycle
	{
	private:
		Engine::EventDispatcher& eventDispatcher;

		std::unordered_map<Data::InputDeviceID, std::unique_ptr<InputDevice>> devices;

		KeyboardInputDevice* keyboard = nullptr;

		MouseInputDevice* mouse = nullptr;

		InputDevice* currentController = nullptr;

		IInputActionHandler* actionHandler = nullptr;

		std::function<void(Data::InputDeviceID)> onDeviceRemoved;


		void AddController(Data::InputDeviceID instanceID, const std::string& name);

		void RemoveController(Data::InputDeviceID instanceID);


		void OnSystemEvent(const Data::SystemEvent& systemEvent) override;

		void BeginFrame() override;

		void PreUpdate() override;


	public:
		DeviceManager(IInputActionHandler* actionHandler, Engine::EventDispatcher& eventDispatcher, std::function<void(Data::InputDeviceID)> onDeviceRemoved = {});

		~DeviceManager() override;

		DeviceManager(const DeviceManager&) = delete;

		DeviceManager(DeviceManager&&) = delete;

		DeviceManager& operator=(const DeviceManager&) = delete;

		DeviceManager& operator=(DeviceManager&&) = delete;


		InputDevice* GetDevice(Data::InputDeviceID id) override;

		std::vector<InputDevice*> GetAllDevices() const override;

		KeyboardInputDevice* Keyboard() override;

		MouseInputDevice* Mouse() override;

		ControllerInputDevice* Controller(Data::InputDeviceID id) override;

		ControllerInputDevice* CurrentController() override;
	};
}