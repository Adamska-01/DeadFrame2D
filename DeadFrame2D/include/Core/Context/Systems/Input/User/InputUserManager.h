#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
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
		DeadFrame2D::Data::InputUserID nextID;

		std::unordered_map<DeadFrame2D::Data::InputUserID, std::unique_ptr<InputUser>> users;

		std::unordered_map<DeadFrame2D::Data::InputDeviceID, InputUser*> pairedDeviceToUser;

		std::unordered_map<DeadFrame2D::Data::InputUserID, std::vector<DeadFrame2D::Data::InputDeviceID>> userToPairedDevices;

		
		void DeviceRemovedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);


	public:
		InputUserManager();

		~InputUserManager();


		InputUser* CreateUser(const std::string& name = "Player");

		InputUser* AutoCreateUserForDevice(InputDevice* device);

		void DestroyUser(DeadFrame2D::Data::InputUserID id);

		InputUser* GetUser(DeadFrame2D::Data::InputUserID id) const;

		std::vector<InputUser*> GetAllUsers() const;

		const std::vector<DeadFrame2D::Data::InputDeviceID>& GetDevicesPairedToUser(DeadFrame2D::Data::InputDeviceID userID) const;

		InputUser* GetUserFromPairedDevice(DeadFrame2D::Data::InputDeviceID deviceID);

		void PairDeviceToUser(InputUser* user, DeadFrame2D::Data::InputDeviceID deviceID);

		void UnpairDevice(InputUser* user, DeadFrame2D::Data::InputDeviceID deviceID);
	};
}