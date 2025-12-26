#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <array>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	class DF2D_API ControllerInputDevice : public InputDevice
	{
	private:
		SDL_GameController* controller;

		InputDeviceID instanceID;

		std::array<DeadFrame2D::Data::InputControlState, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX> buttonStates;

		std::array<DeadFrame2D::Data::InputControlState, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX> axisStates;

		std::unordered_set<int> activeButtons;

		std::unordered_set<int> activeAxes;


		int NormalizedAxisID(int axisID) const;


	public:
		ControllerInputDevice(SDL_GameController* controller, InputDeviceID instanceID);

		virtual ~ControllerInputDevice() override;


		Shared::Models::InputDeviceType Type() const override;
		
		InputDeviceID ID() const override;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetKeyState(int controlId) const override;
	};
}