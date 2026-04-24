#pragma once
#include "Core/Context/Systems/Input/Actions/InputActionView.h"
#include "Data/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <functional>
#include <optional>
#include <string>


namespace DeadFrame2D::Engine
{
	class ComponentHandleBase;
}


namespace DeadFrame2D::Core
{
	class DF2D_API IInputActions
	{
	public:
		virtual ~IInputActions() = default;


		virtual Utilities::ListenerID RegisterAction(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const Engine::ComponentHandleBase& listener,
			const std::function<void(const InputActionView&)>& handler) = 0;

		virtual void DeregisterAction(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const Engine::ComponentHandleBase& listener) = 0;

		virtual void DeregisterActionByID(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			Utilities::ListenerID listenerID) = 0;


		virtual bool EnableActionMap(Data::InputUserID userID, const std::string& name) = 0;

		virtual bool DisableActionMap(Data::InputUserID userID, const std::string& name) = 0;

		virtual bool SwitchToActionMap(Data::InputUserID userID, const std::string& name) = 0;

		virtual std::optional<InputActionView> GetActionState(Data::InputUserID userID, const std::string actionName) = 0;
	};
}