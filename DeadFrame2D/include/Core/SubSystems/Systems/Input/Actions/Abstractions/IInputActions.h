#pragma once
#include "Core/SubSystems/Systems/Input/User/InputUserID.h"
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"
#include "DF2D_API.h"
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
			InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener,
			const std::function<void(const RuntimeInputAction&)>& handler) = 0;

		virtual void DeregisterAction(
			InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener) = 0;

		virtual void DeregisterActionByID(
			InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			DeadFrame2D::Utilities::ListenerID listenerID) = 0;


		virtual bool EnableActionMap(InputUserID userID, const std::string& name) = 0;

		virtual bool DisableActionMap(InputUserID userID, const std::string& name) = 0;

		virtual bool SwitchToActionMap(InputUserID userID, const std::string& name) = 0;

		virtual std::optional<RuntimeInputAction> GetActionState(InputUserID userID, const std::string actionName) = 0;
	};
}