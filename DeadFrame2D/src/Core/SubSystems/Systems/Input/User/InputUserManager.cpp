#include "Core/SubSystems/Systems/Input/User/InputUserManager.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;


	InputUserManager::InputUserManager(std::shared_ptr<DeviceManager> deviceManager)
		: nextID(1), deviceManager(deviceManager)
	{
	}

	std::shared_ptr<InputUser> InputUserManager::CreateUser(const std::string& name)
	{
		auto newUser = std::make_shared<InputUser>(nextID++, name.empty() ? ("User" + std::to_string(nextID - 1)) : name);

		users.emplace(newUser->ID(), newUser);

		EventDispatcher::SendEvent(std::make_shared<InputUserCreatedEvent>(newUser));

		return newUser;
	}

	void InputUserManager::DestroyUser(UserID id)
	{
		auto it = users.find(id);

		if (it == users.end()) 
			return;
		
		auto& userToDestroy = it->second;
		
		// TODO: shared_ptr is not the way to go...
		users.erase(it);
		
		EventDispatcher::SendEvent(std::make_shared<InputUserCreatedEvent>(userToDestroy));
	}

	std::shared_ptr<InputUser> InputUserManager::GetUser(UserID id) const
	{
		auto it = users.find(id);

		if (it == users.end()) 
			return nullptr;
	
		return it->second;
	}

	std::vector<std::shared_ptr<InputUser>> InputUserManager::GetAllUsers() const
	{
		std::vector<std::shared_ptr<InputUser>> out;

		for (auto& p : users)
		{
			out.push_back(p.second);
		}

		return out;
	}

	std::shared_ptr<InputUser> InputUserManager::AutoCreateUserForDevice(std::shared_ptr<InputDevice> device)
	{
		// naive: create a user and pair the device
		auto u = CreateUser();

		PairDeviceToUser(u, device);

		return u;
	}

	void InputUserManager::PairDeviceToUser(std::shared_ptr<InputUser> user, std::shared_ptr<InputDevice> device)
	{
		if (!user || !device) 
			return;
		
		user->PairDevice(device);
	}

	void InputUserManager::UnpairDevice(std::shared_ptr<InputUser> user, DeviceID deviceId)
	{
		if (!user) 
			return;
		
		user->UnpairDevice(deviceId);
	}
}