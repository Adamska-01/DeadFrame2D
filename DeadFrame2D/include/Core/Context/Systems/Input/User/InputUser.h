#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include <string>
#include <vector>


namespace DF2D::Core
{
	class InputUser
	{
	private:
		Data::InputUserID id;

		std::string name;


	public:
		InputUser(Data::InputUserID id, const std::string& name = "User");


		Data::InputUserID ID() const;

		const std::string& Name() const;


		bool IsDevicePaired(Data::InputDeviceID deviceID) const;

		const std::vector<Data::InputDeviceID>& PairedDevices() const;
	};
}