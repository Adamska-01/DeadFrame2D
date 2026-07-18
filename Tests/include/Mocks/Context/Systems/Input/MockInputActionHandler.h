#pragma once
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include <vector>


struct MockInputActionHandler : DF2D::Core::IInputActionHandler
{
	struct BindingCall
	{
		DF2D::Data::InputDeviceID deviceID;

		DF2D::Models::InputControlType controlType;

		int controlID;
	};


	std::vector<BindingCall> calls;


	void ProcessBinding(const DF2D::Core::InputDevice& device, DF2D::Models::InputControlType inputControlType, int controlID) override
	{
		calls.push_back({ device.ID(), inputControlType, controlID });
	}


	int CountCallsFor(int controlID) const
	{
		auto count = 0;

		for (const auto& call : calls)
		{
			if (call.controlID == controlID)
				count++;
		}

		return count;
	}
};