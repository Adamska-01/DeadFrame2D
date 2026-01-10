#pragma once
#include "Data/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <memory>
#include <unordered_map>
#include <vector>


union SDL_Event;


namespace DeadFrame2D::Core
{
	class InputDevice;
	class KeyboardInputDevice;
	class MouseInputDevice;


	/**
	 * @brief Responsible for opening/closing SDL controllers and keeping devices
	 * keyed by instance id. Also holds keyboard and mouse singletons.
	 */
	class DF2D_API DeviceManager
	{
	private:
		std::unordered_map<DeadFrame2D::Data::InputDeviceID, std::shared_ptr<InputDevice>> otherDevices;

		std::shared_ptr<KeyboardInputDevice> keyboard;

		std::shared_ptr<MouseInputDevice> mouse;


		void OpenController(int deviceIndex);

		void CloseController(DeadFrame2D::Data::InputDeviceID instanceId);


	public:
		DeviceManager();

		~DeviceManager();


		void BeginFrame();

		void HandleEvent(const SDL_Event& event);


		InputDevice* GetDevice(DeadFrame2D::Data::InputDeviceID id);

		std::vector<InputDevice*> GetAllDevices() const;

		KeyboardInputDevice* Keyboard();

		MouseInputDevice* Mouse();
	};
}