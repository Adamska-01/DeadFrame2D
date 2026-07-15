#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DF2D::Core
{
	class InputDevice;
	class InputUser;


	class DF2D_API IInputUsers
	{
	public:
		virtual ~IInputUsers() = default;


		virtual InputUser* CreateUser(const std::string& name = "Player") = 0;

		virtual InputUser* AutoCreateUserForDevice(InputDevice* device) = 0;

		virtual void DestroyUser(Data::InputUserID id) = 0;

		virtual InputUser* GetUser(Data::InputUserID id) const = 0;

		virtual std::vector<InputUser*> GetAllUsers() const = 0;

		virtual const std::vector<Data::InputDeviceID>& GetDevicesPairedToUser(Data::InputDeviceID userID) const = 0;

		virtual InputUser* GetUserFromPairedDevice(Data::InputDeviceID deviceID) = 0;

		virtual void PairDeviceToUser(InputUser* user, Data::InputDeviceID deviceID) = 0;

		virtual void UnpairDevice(InputUser* user, Data::InputDeviceID deviceID) = 0;
	};
}
