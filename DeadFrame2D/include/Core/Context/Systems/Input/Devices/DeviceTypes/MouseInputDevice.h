#pragma once
#include "Constants/Input/InputConstants.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/MouseAxisCode.h"
#include "Models/Input/Controls/MouseButtonCode.h"
#include <array>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	/**
	 * @brief Mouse device wrapper.
	 */
	class DF2D_API MouseInputDevice : public InputDevice
	{
	private:
		std::array<Data::InputControlState, (int)Models::MouseButtonCode::COUNT_MAX> buttonStates;

		std::array<Data::InputControlState, (int)Models::MouseAxisCode::COUNT_MAX> axisStates;

		std::unordered_set<uint8_t> activeButtonIDs;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


	public:
		MouseInputDevice(IInputActionHandler* actionHandler);

		~MouseInputDevice() override = default;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		Data::InputControlState GetButtonState(Models::MouseButtonCode code) const;

		Data::InputControlState GetAxisState(Models::MouseAxisCode code) const;

		Vector2F GetMouseDelta() const;

		Vector2F GetWheelDelta() const;
	};
}