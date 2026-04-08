#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <Models/Input/Controls/ControllerAxisCode.h>
#include <Models/Input/Controls/ControllerButtonCode.h>
#include <SDL_gamecontroller.h>
#include <unordered_set>
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API ControllerInputDevice : public InputDevice
	{
	private:
		SDL_GameController* controller;

		DeadFrame2D::Data::InputDeviceID instanceID;

		std::vector<DeadFrame2D::Data::InputControlState> buttonStates;

		std::vector<DeadFrame2D::Data::InputControlState> axisStates;

		std::unordered_set<int> activeButtons;

		std::unordered_set<int> activeAxes;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetButtonState(int buttonID) const override;

		DeadFrame2D::Data::InputControlState GetAxisState(int axisID) const override;


	public:
		ControllerInputDevice(SDL_GameController* controller, DeadFrame2D::Data::InputDeviceID instanceID, IInputActionHandler* actionHandler);

		~ControllerInputDevice() override;


		Shared::Models::InputDeviceType Type() const override;

		DeadFrame2D::Data::InputDeviceID ID() const override;


		DeadFrame2D::Data::InputControlState GetButtonState(Shared::Models::ControllerButtonCode code) const;

		DeadFrame2D::Data::InputControlState GetAxisState(Shared::Models::ControllerAxisCode code) const;
	};
}