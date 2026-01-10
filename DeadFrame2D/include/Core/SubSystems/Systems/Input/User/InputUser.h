#pragma once
#include "Data/Input/InputDeviceID.h"
#include "Data/Input/InputUserID.h"
#include <string>
#include <vector>


namespace DeadFrame2D::Core
{
	class InputUser
	{
	private:
		DeadFrame2D::Data::InputUserID id;

		std::string name;


	public:
		InputUser(DeadFrame2D::Data::InputUserID id, const std::string& name = "User");


		DeadFrame2D::Data::InputUserID ID() const;

		const std::string& Name() const;


		bool IsDevicePaired(DeadFrame2D::Data::InputDeviceID deviceID) const;

		const std::vector<DeadFrame2D::Data::InputDeviceID>& PairedDevices() const;
	};
}