#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include <array>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	/**
	 * @brief Keyboard device wrapper.
	 */
	class DF2D_API KeyboardInputDevice : public InputDevice
	{
	private:
		std::array<DeadFrame2D::Data::InputControlState, SDL_Scancode::SDL_NUM_SCANCODES> states;

		std::unordered_set<SDL_Scancode> activeControlIDs;


	public:
		KeyboardInputDevice();

		virtual ~KeyboardInputDevice() override = default;


		Shared::Models::InputDeviceType Type() const override;

		InputDeviceID ID() const override;


		void BeginFrame() override;

		void ProcessEvent(const SDL_Event& event) override;

		DeadFrame2D::Data::InputControlState GetKeyState(int controlID) const override;
	};
}