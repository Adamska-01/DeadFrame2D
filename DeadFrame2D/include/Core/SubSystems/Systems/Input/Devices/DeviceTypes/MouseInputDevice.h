#pragma once
#include "Constants/Input/InputConstants.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <Core/Math/Vector2.h>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	/**
	 * @brief Mouse device wrapper.
	 */
	class DF2D_API MouseInputDevice : public InputDevice
	{
	private:
		Vector2F mouseDelta;

		Vector2F wheelDelta;

		bool wasMotionThisFrame = false;

		std::array<DeadFrame2D::Data::InputControlState, DeadFrame2D::Constants::InputConstants::MAX_MOUSE_BUTTONS> buttonStates;

		std::unordered_set<uint8_t> activeControlIDs;

		
	public:
		MouseInputDevice();

		virtual ~MouseInputDevice() override = default;


		Shared::Models::InputDeviceType Type() const override;

		DeviceID ID() const override;


		void BeginFrame(InputActionResolver* inputActionResolver) override;

		void ProcessEvent(const SDL_Event& event, InputActionResolver* inputActionResolver) override;

		DeadFrame2D::Data::InputControlState GetKeyState(int controlID) const override;


		Vector2F GetMouseDelta() const;

		Vector2F GetWheelDelta() const;
	};
}