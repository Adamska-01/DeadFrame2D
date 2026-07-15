#pragma once
#include "Core/Context/Systems/Input/Actions/InputActionView.h"
#include "Data/Systems/Input/ActionListenerEntry.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <functional>
#include <string>
#include <vector>


namespace DF2D::Core
{
	class Input;
}


namespace DF2D::Engine
{
	class DF2D_API PlayerInput : public GameComponent
	{
		TYPE_INFO(PlayerInput, GameComponent);


	private:
		Core::Input* input = nullptr;

		Data::InputUserID userID = -1;

		std::string playerName;

		std::vector<Data::ActionListenerEntry> registeredListeners;

		// Calls made before Init (the input system is wired in Init) are replayed there
		std::vector<std::function<void()>> pendingOperations;


	public:
		PlayerInput(const std::string& playerName);

		virtual ~PlayerInput() override;


		virtual void Init() override;


		Utilities::ListenerID RegisterAction(
			const std::string& actionMapName,
			const std::string& actionName,
			const ComponentHandleBase& listener,
			const std::function<void(const Core::InputActionView&)>& handler);

		void DeregisterAction(const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener);

		void DeregisterActionByID(const std::string& actionMapName, const std::string& actionName, Utilities::ListenerID listenerID);

		bool EnableActionMap(const std::string& actionMapName);

		bool DisableActionMap(const std::string& actionMapName);

		bool SwitchToActionMap(const std::string& actionMapName);

		const std::string& GetPlayerName() const;
	};
}