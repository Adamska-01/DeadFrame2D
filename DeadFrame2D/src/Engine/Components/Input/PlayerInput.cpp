#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Engine/Components/Input/PlayerInput.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	PlayerInput::PlayerInput(const std::string& playerName)
		: playerName(playerName)
	{
		userID = Input::Users()->CreateUser(playerName)->ID();
	}

	PlayerInput::~PlayerInput()
	{
		for (const auto& e : registeredListeners)
		{
			Input::Actions()->DeregisterActionByID(userID, e.mapName, e.actionName, e.listenerID);
		}

		registeredListeners.clear();

		Input::Users()->DestroyUser(userID);
	}

	ListenerID PlayerInput::RegisterAction(
		const std::string& actionMapName,
		const std::string& actionName,
		const ComponentHandleBase& listener,
		const std::function<void(const InputActionView&)>& handler)
	{
		auto id = Input::Actions()->RegisterAction(userID, actionMapName, actionName, listener, handler);

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
		Input::Actions()->DeregisterAction(userID, actionMapName, actionName, listener);

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
		Input::Actions()->DeregisterActionByID(userID, actionMapName, actionName, listenerID);

		std::erase_if(
			registeredListeners,
			[&](const ActionListenerEntry& e)
			{
				return e.mapName == actionMapName
					&& e.actionName == actionName
					&& e.listenerID == listenerID;
			});
	}

	bool PlayerInput::EnableActionMap(const std::string& actionMapName) const
	{
		return Input::Actions()->EnableActionMap(userID, actionMapName);
	}

	bool PlayerInput::DisableActionMap(const std::string& actionMapName) const
	{
		return Input::Actions()->DisableActionMap(userID, actionMapName);
	}

	bool PlayerInput::SwitchToActionMap(const std::string& actionMapName) const
	{
		return Input::Actions()->SwitchToActionMap(userID, actionMapName);
	}

	const std::string& PlayerInput::GetPlayerName() const
	{
		return playerName;
	}
}