#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/ControllerAxisCode.h"
#include "Models/Input/Controls/ControllerButtonCode.h"
#include <SDL_gamecontroller.h>
#include <unordered_set>
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API ControllerInputDevice : public InputDevice
	{
	private:
		SDL_GameController* controller;

		Data::InputDeviceID instanceID;

		std::vector<Data::InputControlState> buttonStates;

		std::vector<Data::InputControlState> axisStates;

		std::unordered_set<int> activeButtons;

		std::unordered_set<int> activeAxes;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


	public:
		ControllerInputDevice(SDL_GameController* controller, Data::InputDeviceID instanceID, IInputActionHandler* actionHandler);

		~ControllerInputDevice() override;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		Data::InputControlState GetButtonState(Models::ControllerButtonCode code) const;

		Data::InputControlState GetAxisState(Models::ControllerAxisCode code) const;
	};
}