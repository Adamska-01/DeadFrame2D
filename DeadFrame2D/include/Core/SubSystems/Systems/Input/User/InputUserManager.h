#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceManager.h"
#include "Core/SubSystems/Systems/Input/User/InputUser.h"
#include <map>
#include <memory>


namespace DeadFrame2D::Core
{
	class InputUserManager
	{
	private:
		UserID nextID;

		std::map<UserID, std::shared_ptr<InputUser>> users;
		
		std::shared_ptr<DeviceManager> deviceManager;


	public:
		InputUserManager(std::shared_ptr<DeviceManager> deviceManager);

		~InputUserManager() = default;


		std::shared_ptr<InputUser> CreateUser(const std::string& name = "");

		void DestroyUser(UserID id);

		std::shared_ptr<InputUser> GetUser(UserID id) const;

		std::vector<std::shared_ptr<InputUser>> GetAllUsers() const;

		std::shared_ptr<InputUser> AutoCreateUserForDevice(std::shared_ptr<InputDevice> device);

		void PairDeviceToUser(std::shared_ptr<InputUser> user, std::shared_ptr<InputDevice> device);

		void UnpairDevice(std::shared_ptr<InputUser> user, DeviceID deviceId);
	};
}