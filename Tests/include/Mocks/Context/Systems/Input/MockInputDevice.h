#pragma once
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include <unordered_map>


struct MockInputDevice : DF2D::Core::InputDevice
{
	DF2D::Models::InputDeviceType type{DF2D::Models::InputDeviceType::KEYBOARD};

	DF2D::Data::InputDeviceID id{-1};

	std::unordered_map<int, DF2D::Data::InputControlState> buttonStates;

	std::unordered_map<int, DF2D::Data::InputControlState> axisStates;


	MockInputDevice(DF2D::Core::IInputActionHandler* actionHandler)
		: InputDevice("Fake", actionHandler)
	{
	}


	DF2D::Models::InputDeviceType Type() const override
	{
		return type;
	}

	DF2D::Data::InputDeviceID ID() const override
	{
		return id;
	}

	void BeginFrame() override
	{
	}

	bool HandleEvent(const DF2D::Data::SystemEvent& systemEvent) override
	{
		return false;
	}

	DF2D::Data::InputControlState GetButtonState(int buttonID) const override
	{
		auto it = buttonStates.find(buttonID);

		return it != buttonStates.end() ? it->second : DF2D::Data::InputControlState{};
	}

	DF2D::Data::InputControlState GetAxisState(int axisID) const override
	{
		auto it = axisStates.find(axisID);

		return it != axisStates.end() ? it->second : DF2D::Data::InputControlState{};
	}


	void SetButton(int buttonID, float value)
	{
		buttonStates[buttonID] = DF2D::Data::InputControlState{ .value = value };
	}

	void SetAxis(int axisID, float value)
	{
		axisStates[axisID] = DF2D::Data::InputControlState{ .value = value };
	}
};