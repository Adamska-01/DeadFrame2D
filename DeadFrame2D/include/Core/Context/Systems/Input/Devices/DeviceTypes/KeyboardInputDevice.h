#pragma once
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"
#include <unordered_set>
#include <vector>


namespace DF2D::Core
{
	/**
	 * @brief Keyboard device; state keyed by KeyboardKeyCode.
	 */
	class DF2D_API KeyboardInputDevice : public InputDevice
	{
	private:
		std::vector<Data::InputControlState> states;

		std::unordered_set<uint16_t> activeControlIDs;


		void BeginFrame() override;

		bool HandleEvent(const Data::SystemEvent& systemEvent) override;


		void HandleKey(Models::KeyboardKeyCode key, bool pressed);


	public:
		KeyboardInputDevice(IInputActionHandler* actionHandler);

		~KeyboardInputDevice() override = default;


		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		Data::InputControlState GetButtonState(Models::KeyboardKeyCode code) const;
	};
}