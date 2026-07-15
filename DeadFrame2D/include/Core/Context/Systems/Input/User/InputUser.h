#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DF2D::Core
{
	class InputUserManager;


	class DF2D_API InputUser
	{
	private:
		Data::InputUserID id;

		InputUserManager* manager;

		std::string name;


	public:
		InputUser(Data::InputUserID id, InputUserManager* manager, const std::string& name = "User");


		Data::InputUserID ID() const;

		const std::string& Name() const;


		bool IsDevicePaired(Data::InputDeviceID deviceID) const;

		const std::vector<Data::InputDeviceID>& PairedDevices() const;
	};
}