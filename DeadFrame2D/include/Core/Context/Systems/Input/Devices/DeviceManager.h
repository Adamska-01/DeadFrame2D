#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/Context/Systems/Input/Devices/Abstractions/IInputDeviceProvider.h"
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "Data/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <memory>
#include <unordered_map>
#include <vector>


union SDL_Event;


namespace DeadFrame2D::Core
{
	class IInputActionHandler;


	/**
	 * @brief Responsible for opening/closing SDL controllers and keeping devices
	 * keyed by instance id. Also holds keyboard and mouse singletons.
	 */
	class DF2D_API DeviceManager : public IEventProcessor, public IInputFrameLifecycle, public IInputDeviceProvider
	{
	private:
		std::unordered_map<DeadFrame2D::Data::InputDeviceID, std::shared_ptr<InputDevice>> devices;

		InputDevice* currentController;

		IInputActionHandler* actionHandler;


		void OpenController(int deviceIndex);

		void CloseController(DeadFrame2D::Data::InputDeviceID instanceId);


		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


		void BeginFrame() override;

		void PreUpdate() override;


	public:
		DeviceManager(IInputActionHandler* actionHandler);

		~DeviceManager() override;


		InputDevice* GetDevice(DeadFrame2D::Data::InputDeviceID id) override;

		std::vector<InputDevice*> GetAllDevices() const override;

		KeyboardInputDevice* Keyboard() override;

		MouseInputDevice* Mouse() override;

		ControllerInputDevice* Controller(DeadFrame2D::Data::InputDeviceID id) override;

		ControllerInputDevice* CurrentController() override;
	};
}