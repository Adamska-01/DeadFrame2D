#pragma once
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/MouseAxisCode.h"
#include "Models/Input/Controls/MouseButtonCode.h"
#include <array>
#include <unordered_set>


namespace DF2D::Core
{
	/**
	 * @brief Mouse device; state keyed by MouseButtonCode / MouseAxisCode.
	 *
	 * Motion and wheel axes hold per-frame deltas; the position axes hold the
	 * absolute cursor position and are not reset between frames.
	 */
	class DF2D_API MouseInputDevice : public InputDevice
	{
	private:
		std::array<Data::InputControlState, (int)Models::MouseButtonCode::COUNT_MAX> buttonStates;

		std::array<Data::InputControlState, (int)Models::MouseAxisCode::COUNT_MAX> axisStates;

		std::unordered_set<uint8_t> activeButtonIDs;


		void UpdateMotionAxisPhase(Models::MouseAxisCode axis);


		void BeginFrame() override;

		bool HandleEvent(const Data::SystemEvent& systemEvent) override;


		void HandleButton(Models::MouseButtonCode button, bool pressed, const Vector2F& position);

		void HandleMove(const Vector2F& position, const Vector2F& delta);

		void HandleWheel(const Vector2F& delta);


	public:
		MouseInputDevice(IInputActionHandler* actionHandler);

		~MouseInputDevice() override = default;


		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		Data::InputControlState GetButtonState(Models::MouseButtonCode code) const;

		Data::InputControlState GetAxisState(Models::MouseAxisCode code) const;

		Vector2F GetMousePosition() const;

		Vector2F GetMouseDelta() const;

		Vector2F GetWheelDelta() const;
	};
}