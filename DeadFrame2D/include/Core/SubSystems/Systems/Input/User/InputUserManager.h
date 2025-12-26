#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/InputDeviceID.h"
#include "Core/SubSystems/Systems/Input/User/InputUserID.h"
#include <memory>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	class DispatchableEvent;
}


namespace DeadFrame2D::Core
{
	class InputDevice;
	class InputUser;


	class InputUserManager
	{
	private:
		InputUserID nextID;

		std::unordered_map<InputUserID, std::unique_ptr<InputUser>> users;

		std::unordered_map<InputDeviceID, InputUser*> pairedDeviceToUser;

		std::unordered_map<InputUserID, std::vector<InputDeviceID>> userToPairedDevices;

		
		void DeviceRemovedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);


	public:
		InputUserManager();

		~InputUserManager();


		InputUser* CreateUser(const std::string& name = "Player");

		InputUser* AutoCreateUserForDevice(InputDevice* device);

		void DestroyUser(InputUserID id);

		InputUser* GetUser(InputUserID id) const;

		std::vector<InputUser*> GetAllUsers() const;

		const std::vector<InputDeviceID>& GetDevicesPairedToUser(InputDeviceID userID) const;

		InputUser* GetUserFromPairedDevice(InputDeviceID deviceID);

		void PairDeviceToUser(InputUser* user, InputDeviceID deviceID);

		void UnpairDevice(InputUser* user, InputDeviceID deviceID);
	};
}