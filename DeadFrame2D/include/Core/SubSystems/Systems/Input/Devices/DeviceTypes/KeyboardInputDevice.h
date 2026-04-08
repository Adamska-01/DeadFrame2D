#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <vector>
#include <Models/Input/Controls/KeyboardKeyCode.h>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	/**
	 * @brief Keyboard device wrapper.
	 */
	class DF2D_API KeyboardInputDevice : public InputDevice
	{
	private:
		std::vector<DeadFrame2D::Data::InputControlState> states;

		std::unordered_set<uint16_t> activeControlIDs;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetButtonState(int buttonID) const override;

		DeadFrame2D::Data::InputControlState GetAxisState(int axisID) const override;


	public:
		KeyboardInputDevice(IInputActionHandler* actionHandler);

		~KeyboardInputDevice() override = default;


		Shared::Models::InputDeviceType Type() const override;

		DeadFrame2D::Data::InputDeviceID ID() const override;


		DeadFrame2D::Data::InputControlState GetButtonState(Shared::Models::KeyboardKeyCode code) const;
	};
}