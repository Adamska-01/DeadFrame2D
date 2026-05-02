#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include <memory>
#include <unordered_map>


namespace DF2D::Engine
{
	class DispatchableEvent;
}


namespace DF2D::Core
{
	class InputDevice;
	class InputUser;


	class InputUserManager
	{
	private:
		Data::InputUserID nextID;

		std::unordered_map<Data::InputUserID, std::unique_ptr<InputUser>> users;

		std::unordered_map<Data::InputDeviceID, InputUser*> pairedDeviceToUser;

		std::unordered_map<Data::InputUserID, std::vector<Data::InputDeviceID>> userToPairedDevices;

		
		void DeviceRemovedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);


	public:
		InputUserManager();

		~InputUserManager();


		InputUser* CreateUser(const std::string& name = "Player");

		InputUser* AutoCreateUserForDevice(InputDevice* device);

		void DestroyUser(Data::InputUserID id);

		InputUser* GetUser(Data::InputUserID id) const;

		std::vector<InputUser*> GetAllUsers() const;

		const std::vector<Data::InputDeviceID>& GetDevicesPairedToUser(Data::InputDeviceID userID) const;

		InputUser* GetUserFromPairedDevice(Data::InputDeviceID deviceID);

		void PairDeviceToUser(InputUser* user, Data::InputDeviceID deviceID);

		void UnpairDevice(InputUser* user, Data::InputDeviceID deviceID);
	};
}