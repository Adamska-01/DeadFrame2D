#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/InputDeviceID.h"
#include "Core/SubSystems/Systems/Input/User/InputUserID.h"
#include <string>
#include <vector>


namespace DeadFrame2D::Core
{
	class InputUser
	{
	private:
		InputUserID id;

		std::string name;


	public:
		InputUser(InputUserID id, const std::string& name = "User");


		InputUserID ID() const;

		const std::string& Name() const;


		bool IsDevicePaired(InputDeviceID deviceID) const;

		const std::vector<InputDeviceID>& PairedDevices() const;
	};
}