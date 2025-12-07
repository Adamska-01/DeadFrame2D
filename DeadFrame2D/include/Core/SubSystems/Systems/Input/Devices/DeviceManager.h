#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include <memory>
#include <unordered_map>
#include <vector>


namespace DeadFrame2D::Core
{
	class InputActionResolver;


	/**
	 * @brief Responsible for opening/closing SDL controllers and keeping devices
	 * keyed by instance id. Also holds keyboard and mouse singletons.
	 */
	class DeviceManager
	{
	private:
		std::shared_ptr<InputActionResolver> inputActionResolver;


		std::unordered_map<DeviceID, std::shared_ptr<InputDevice>> otherDevices;

		std::shared_ptr<KeyboardInputDevice> keyboard;

		std::shared_ptr<MouseInputDevice> mouse;


		void OpenController(int deviceIndex);

		void CloseController(DeviceID instanceId);


	public:
		DeviceManager(std::shared_ptr<InputActionResolver> inputActionResolver);

		~DeviceManager();


		void BeginFrame();

		void HandleEvent(const SDL_Event& event);


		std::shared_ptr<InputDevice> GetDevice(DeviceID id);

		std::vector<std::shared_ptr<InputDevice>> GetAllDevices() const;

		std::shared_ptr<KeyboardInputDevice> Keyboard();

		std::shared_ptr<MouseInputDevice> Mouse();
	};
}