#pragma once
#include "Data/Input/InputUserID.h"
#include "Data/Input/ActionListenerEntry.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <functional>
#include <string>


namespace DeadFrame2D::Core
{
	class RuntimeInputAction;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API PlayerInput : public GameComponent
	{
		TYPE_INFO(PlayerInput, GameComponent);


	private:
		DeadFrame2D::Data::InputUserID userID;

		std::string playerName;

		std::vector<DeadFrame2D::Data::ActionListenerEntry> registeredListeners;


	public:
		PlayerInput(const std::string& playerName);

		virtual ~PlayerInput() override;


		DeadFrame2D::Utilities::ListenerID RegisterAction(const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener, const std::function<void(const DeadFrame2D::Core::RuntimeInputAction&)>& handler);

		void DeregisterAction(const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener);

		void DeregisterActionByID(const std::string& actionMapName, const std::string& actionName, DeadFrame2D::Utilities::ListenerID listenerID);

		bool EnableActionMap(const std::string& actionMapName) const;

		bool DisableActionMap(const std::string& actionMapName) const;

		bool SwitchToActionMap(const std::string& actionMapName) const;

		const std::string& GetPlayerName() const;
	};
}