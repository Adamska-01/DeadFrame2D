#pragma once
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"
#include <unordered_set>
#include <vector>


namespace DF2D::Core
{
	/**
	 * @brief Keyboard device wrapper.
	 */
	class DF2D_API KeyboardInputDevice : public InputDevice
	{
	private:
		std::vector<Data::InputControlState> states;

		std::unordered_set<uint16_t> activeControlIDs;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


	public:
		KeyboardInputDevice(IInputActionHandler* actionHandler);

		~KeyboardInputDevice() override = default;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		Data::InputControlState GetButtonState(Models::KeyboardKeyCode code) const;
	};
}