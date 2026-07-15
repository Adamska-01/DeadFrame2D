#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Input/InputUserCreatedEvent.h"
#include "Engine/Events/Context/Input/InputUserDestroyedEvent.h"
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;


	InputUserManager::InputUserManager()
		: nextID(0)
	{
	}

	InputUserManager::~InputUserManager()
	{
	}

	void InputUserManager::SetUserLifecycleHooks(std::function<void(InputUserID)> onCreated, std::function<void(InputUserID)> onDestroyed)
	{
		onUserCreated = std::move(onCreated);
		onUserDestroyed = std::move(onDestroyed);
	}

	InputUser* InputUserManager::CreateUser(const std::string& name)
	{
		auto userID = nextID++;

		auto newUser = std::make_unique<InputUser>(userID, this, name.empty() ? ("User" + std::to_string(userID)) : name);

		auto userPtr = newUser.get();

		users.emplace(newUser->ID(), std::move(newUser));

		if (GetUserFromPairedDevice(DefaultDeviceIDs::KEYBOARD) == nullptr)
		{
			PairDeviceToUser(userPtr, DefaultDeviceIDs::KEYBOARD);
		}
		if (GetUserFromPairedDevice(DefaultDeviceIDs::MOUSE) == nullptr)
		{
			PairDeviceToUser(userPtr, DefaultDeviceIDs::MOUSE);
		}

		if (onUserCreated)
		{
			onUserCreated(userPtr->ID());
		}

		std::cout << "[Input] User created: " << userPtr->Name() << " (ID: " << userPtr->ID() << ")" << std::endl;

		EventDispatcher::SendEvent(std::make_shared<InputUserCreatedEvent>(userPtr->ID(), userPtr->Name()));

		return userPtr;
	}

	InputUser* InputUserManager::AutoCreateUserForDevice(InputDevice* device)
	{
		if (device == nullptr)
			return nullptr;

		// naive: create a user and pair the device
		auto newUser = CreateUser();

		PairDeviceToUser(newUser, device->ID());

		return newUser;
	}

	void InputUserManager::DestroyUser(InputUserID id)
	{
		auto it = users.find(id);

		if (it == users.end())
			return;

		auto userToDestroyPtr = it->second.get();

		// Remove all paired devices
		if (auto devicesIt = userToPairedDevices.find(userToDestroyPtr->ID()); devicesIt != userToPairedDevices.end())
		{
			for (auto deviceID : devicesIt->second)
			{
				pairedDeviceToUser.erase(deviceID);
			}

			userToPairedDevices.erase(devicesIt);
		}

		std::erase_if(
			pairedDeviceToUser,
			[userToDestroyPtr](const auto& pair)
			{
				return pair.second == userToDestroyPtr;
			});

		if (onUserDestroyed)
		{
			onUserDestroyed(userToDestroyPtr->ID());
		}

		std::cout << "[Input] User destroyed: " << userToDestroyPtr->Name() << " (ID: " << userToDestroyPtr->ID() << ")" << std::endl;

		EventDispatcher::SendEvent(std::make_shared<InputUserDestroyedEvent>(userToDestroyPtr->ID(), userToDestroyPtr->Name()));

		users.erase(it);
	}

	InputUser* InputUserManager::GetUser(InputUserID id) const
	{
		auto it = users.find(id);

		return it != users.end() ? it->second.get() : nullptr;
	}

	std::vector<InputUser*> InputUserManager::GetAllUsers() const
	{
		std::vector<InputUser*> out;

		out.reserve(users.size());

		for (auto& p : users)
		{
			out.push_back(p.second.get());
		}

		return out;
	}

	const std::vector<InputDeviceID>& InputUserManager::GetDevicesPairedToUser(InputDeviceID userID) const
	{
		static const std::vector<InputDeviceID> empty;

		auto it = userToPairedDevices.find(userID);

		return it != userToPairedDevices.end() ? it->second : empty;
	}

	InputUser* InputUserManager::GetUserFromPairedDevice(InputDeviceID deviceID)
	{
		auto it = pairedDeviceToUser.find(deviceID);

		if (it == pairedDeviceToUser.end())
			return nullptr;

		return it->second;
	}

	std::optional<InputUserID> InputUserManager::GetUserIDFromPairedDevice(InputDeviceID deviceID) const
	{
		auto it = pairedDeviceToUser.find(deviceID);

		if (it == pairedDeviceToUser.end())
			return std::nullopt;

		return it->second->ID();
	}

	void InputUserManager::PairDeviceToUser(InputUser* user, InputDeviceID deviceID)
	{
		if (!user)
			return;

		pairedDeviceToUser[deviceID] = user;

		auto& devices = userToPairedDevices[user->ID()];

		if (std::find(devices.begin(), devices.end(), deviceID) == devices.end())
		{
			devices.push_back(deviceID);
		}
	}

	void InputUserManager::UnpairDevice(InputUser* user, InputDeviceID deviceID)
	{
		if (!user)
			return;

		pairedDeviceToUser.erase(deviceID);

		if (auto it = userToPairedDevices.find(user->ID()); it != userToPairedDevices.end())
		{
			auto& devices = it->second;

			devices.erase(
				std::remove(
					devices.begin(),
					devices.end(),
					deviceID),
				devices.end());

			if (devices.empty())
			{
				userToPairedDevices.erase(it);
			}
		}
	}
}