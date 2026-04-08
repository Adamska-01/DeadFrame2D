#pragma once
#include "Constants/Input/InputConstants.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <array>
#include <Models/Input/Controls/MouseButtonCode.h>
#include <Models/Input/Controls/MouseAxisCode.h>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	/**
	 * @brief Mouse device wrapper.
	 */
	class DF2D_API MouseInputDevice : public InputDevice
	{
	private:
		std::array<DeadFrame2D::Data::InputControlState, (int)Shared::Models::MouseButtonCode::COUNT_MAX> buttonStates;

		std::array<DeadFrame2D::Data::InputControlState, (int)Shared::Models::MouseAxisCode::COUNT_MAX> axisStates;

		std::unordered_set<uint8_t> activeButtonIDs;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetButtonState(int buttonID) const override;

		DeadFrame2D::Data::InputControlState GetAxisState(int axisID) const override;


	public:
		MouseInputDevice(IInputActionHandler* actionHandler);

		~MouseInputDevice() override = default;


		Shared::Models::InputDeviceType Type() const override;

		DeadFrame2D::Data::InputDeviceID ID() const override;


		DeadFrame2D::Data::InputControlState GetButtonState(Shared::Models::MouseButtonCode code) const;

		DeadFrame2D::Data::InputControlState GetAxisState(Shared::Models::MouseAxisCode code) const;

		Vector2F GetMouseDelta() const;

		Vector2F GetWheelDelta() const;
	};
}