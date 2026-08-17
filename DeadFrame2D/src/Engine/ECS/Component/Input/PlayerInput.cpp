#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Engine/ECS/Component/Input/PlayerInput.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	PlayerInput::PlayerInput(const std::string& playerName)
		: playerName(playerName)
	{
	}

	PlayerInput::~PlayerInput()
	{
		if (input == nullptr)
			return;

		for (const auto& e : registeredListeners)
		{
			input->Actions()->DeregisterActionByID(userID, e.mapName, e.actionName, e.listenerID);
		}

		registeredListeners.clear();

		input->Users()->DestroyUser(userID);
	}

	void PlayerInput::Init()
	{
		GameComponent::Init();

		input = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().input, NAME_OF(input));

		userID = input->Users()->CreateUser(playerName)->ID();

		auto pending = std::move(pendingOperations);

		pendingOperations.clear();

		for (auto& operation : pending)
		{
			operation();
		}
	}

	ListenerID PlayerInput::RegisterAction(
		const std::string& actionMapName,
		const std::string& actionName,
		const ComponentHandleBase& listener,
		const std::function<void(const InputActionView&)>& handler)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName, actionName, listener, handler]()
				{
					RegisterAction(actionMapName, actionName, listener, handler);
				});

			return -1;
		}

		auto id = input->Actions()->RegisterAction(userID, actionMapName, actionName, listener, handler);

		registeredListeners.push_back(
			ActionListenerEntry
			{
				.mapName = actionMapName,
				.actionName = actionName,
				.listener = listener,
				.listenerID = id
			});

		return id;
	}

	void PlayerInput::DeregisterAction(const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName, actionName, listener]()
				{
					DeregisterAction(actionMapName, actionName, listener);
				});

			return;
		}

		input->Actions()->DeregisterAction(userID, actionMapName, actionName, listener);

		std::erase_if(
			registeredListeners,
			[&](const ActionListenerEntry& e)
			{
				return e.mapName == actionMapName
					&& e.actionName == actionName
					&& e.listener == listener;
			});
	}

	void PlayerInput::DeregisterActionByID(const std::string& actionMapName, const std::string& actionName, ListenerID listenerID)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName, actionName, listenerID]()
				{
					DeregisterActionByID(actionMapName, actionName, listenerID);
				});

			return;
		}

		input->Actions()->DeregisterActionByID(userID, actionMapName, actionName, listenerID);

		std::erase_if(
			registeredListeners,
			[&](const ActionListenerEntry& e)
			{
				return e.mapName == actionMapName
					&& e.actionName == actionName
					&& e.listenerID == listenerID;
			});
	}

	bool PlayerInput::EnableActionMap(const std::string& actionMapName)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName]()
				{
					EnableActionMap(actionMapName);
				});

			return false;
		}

		return input->Actions()->EnableActionMap(userID, actionMapName);
	}

	bool PlayerInput::DisableActionMap(const std::string& actionMapName)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName]()
				{
					DisableActionMap(actionMapName);
				});

			return false;
		}

		return input->Actions()->DisableActionMap(userID, actionMapName);
	}

	bool PlayerInput::SwitchToActionMap(const std::string& actionMapName)
	{
		if (input == nullptr)
		{
			pendingOperations.push_back(
				[this, actionMapName]()
				{
					SwitchToActionMap(actionMapName);
				});

			return false;
		}

		return input->Actions()->SwitchToActionMap(userID, actionMapName);
	}

	const std::string& PlayerInput::GetPlayerName() const
	{
		return playerName;
	}
}