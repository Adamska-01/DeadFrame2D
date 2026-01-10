#pragma once
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
	class RuntimeInputAction;


	class DF2D_API IInputActions
	{
	public:
		virtual ~IInputActions() = default;


		virtual DeadFrame2D::Utilities::ListenerID RegisterAction(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener,
			const std::function<void(const RuntimeInputAction&)>& handler) = 0;

		virtual void DeregisterAction(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener) = 0;

		virtual void DeregisterActionByID(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			DeadFrame2D::Utilities::ListenerID listenerID) = 0;


		virtual bool EnableActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) = 0;

		virtual bool DisableActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) = 0;

		virtual bool SwitchToActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) = 0;

		virtual std::optional<RuntimeInputAction> GetActionState(DeadFrame2D::Data::InputUserID userID, const std::string actionName) = 0;
	};
}