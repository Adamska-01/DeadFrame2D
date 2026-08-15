#pragma once
#include "Core/Context/Systems/Input/User/Abstractions/IInputUsers.h"
#include "Core/Context/Systems/Input/User/Abstractions/IUserDevicePairings.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace DF2D::Engine
{
	class EventDispatcher;
}


namespace DF2D::Core
{
	class DF2D_API InputUserManager : public IInputUsers, public IUserDevicePairings
	{
	private:
		Engine::EventDispatcher& eventDispatcher;

		Data::InputUserID nextID;

		std::unordered_map<Data::InputUserID, std::unique_ptr<InputUser>> users;

		std::unordered_map<Data::InputDeviceID, InputUser*> pairedDeviceToUser;

		std::unordered_map<Data::InputUserID, std::vector<Data::InputDeviceID>> userToPairedDevices;

		std::function<void(Data::InputUserID)> onUserCreated;

		std::function<void(Data::InputUserID)> onUserDestroyed;


	public:
		/**
		 * @brief Lifecycle hooks are invoked before the matching engine events are
		 * broadcast; they are wired once at construction (by the Input system).
		 */
		InputUserManager(Engine::EventDispatcher& eventDispatcher, std::function<void(Data::InputUserID)> onUserCreated = {}, std::function<void(Data::InputUserID)> onUserDestroyed = {});

		~InputUserManager() override;

		InputUserManager(const InputUserManager&) = delete;

		InputUserManager(InputUserManager&&) = delete;

		InputUserManager& operator=(const InputUserManager&) = delete;

		InputUserManager& operator=(InputUserManager&&) = delete;


		InputUser* CreateUser(const std::string& name = "Player") override;

		InputUser* AutoCreateUserForDevice(InputDevice* device) override;

		void DestroyUser(Data::InputUserID id) override;

		InputUser* GetUser(Data::InputUserID id) const override;

		std::vector<InputUser*> GetAllUsers() const override;

		const std::vector<Data::InputDeviceID>& GetDevicesPairedToUser(Data::InputDeviceID userID) const override;

		InputUser* GetUserFromPairedDevice(Data::InputDeviceID deviceID) override;

		std::optional<Data::InputUserID> GetUserIDFromPairedDevice(Data::InputDeviceID deviceID) const override;

		void PairDeviceToUser(InputUser* user, Data::InputDeviceID deviceID) override;

		void UnpairDevice(InputUser* user, Data::InputDeviceID deviceID) override;
	};
}