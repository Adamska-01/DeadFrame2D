#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/Context/Systems/Input/Devices/Abstractions/IInputDeviceProvider.h"
#include "Core/Services/Events/Abstractions/IEventProcessor.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <memory>
#include <unordered_map>
#include <vector>


union SDL_Event;


namespace DF2D::Core
{
	class IInputActionHandler;


	/**
	 * @brief Responsible for opening/closing SDL controllers and keeping devices
	 * keyed by instance id. Also holds keyboard and mouse singletons.
	 */
	class DF2D_API DeviceManager : public IEventProcessor, public IInputFrameLifecycle, public IInputDeviceProvider
	{
	private:
		std::unordered_map<Data::InputDeviceID, std::shared_ptr<InputDevice>> devices;

		InputDevice* currentController;

		IInputActionHandler* actionHandler;


		void OpenController(int deviceIndex);

		void CloseController(Data::InputDeviceID instanceId);


		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


		void BeginFrame() override;

		void PreUpdate() override;


	public:
		DeviceManager(IInputActionHandler* actionHandler);

		~DeviceManager() override;


		InputDevice* GetDevice(Data::InputDeviceID id) override;

		std::vector<InputDevice*> GetAllDevices() const override;

		KeyboardInputDevice* Keyboard() override;

		MouseInputDevice* Mouse() override;

		ControllerInputDevice* Controller(Data::InputDeviceID id) override;

		ControllerInputDevice* CurrentController() override;
	};
}