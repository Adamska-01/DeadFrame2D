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

		DeviceID instanceID;

		std::array<DeadFrame2D::Data::InputControlState, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX> buttonStates;

		std::array<DeadFrame2D::Data::InputControlState, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX> axisStates;

		std::unordered_set<int> activeButtons;

		std::unordered_set<int> activeAxes;


		int NormalizedAxisID(int axisID) const;


	public:
		ControllerInputDevice(SDL_GameController* controller, DeviceID instanceID);

		virtual ~ControllerInputDevice() override;


		Shared::Models::InputDeviceType Type() const override;
		
		DeviceID ID() const override;


		void BeginFrame() override;

		int ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetKeyState(int controlId) const override;
	};
}