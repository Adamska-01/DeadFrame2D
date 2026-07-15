#pragma once
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "DF2D_API.h"
#include "Models/Input/Controls/ControllerAxisCode.h"
#include "Models/Input/Controls/ControllerButtonCode.h"
#include <string>
#include <unordered_set>
#include <vector>


namespace DF2D::Core
{
	/**
	 * @brief Controller device; state keyed by ControllerButtonCode / ControllerAxisCode.
	 *
	 * The native controller handle is owned by the event source backend; this
	 * class only holds per-control state fed through engine events.
	 */
	class DF2D_API ControllerInputDevice : public InputDevice
	{
	private:
		Data::InputDeviceID instanceID;

		std::vector<Data::InputControlState> buttonStates;

		std::vector<Data::InputControlState> axisStates;

		std::unordered_set<int> activeButtons;

		std::unordered_set<int> activeAxes;


	public:
		ControllerInputDevice(Data::InputDeviceID instanceID, const std::string& name, IInputActionHandler* actionHandler);

		~ControllerInputDevice() override = default;


		void BeginFrame() override;

		Data::InputControlState GetButtonState(int buttonID) const override;

		Data::InputControlState GetAxisState(int axisID) const override;


		Models::InputDeviceType Type() const override;

		Data::InputDeviceID ID() const override;


		void HandleButton(Models::ControllerButtonCode button, bool pressed);

		void HandleAxis(Models::ControllerAxisCode axis, float normalizedValue);


		Data::InputControlState GetButtonState(Models::ControllerButtonCode code) const;

		Data::InputControlState GetAxisState(Models::ControllerAxisCode code) const;
	};
}