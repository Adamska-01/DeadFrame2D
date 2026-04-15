#pragma once
#include "Core/SubSystems/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActions.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionMapIndex.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "Data/Input/InputUserID.h"
#include "DF2D_API.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>


namespace DeadFrame2D::Models
{
	struct Binding;
}

namespace DeadFrame2D::Engine
{
	class DispatchableEvent;
	class ComponentHandleBase;
	class PlayerInput;
}


namespace DeadFrame2D::Core
{
	class RuntimeInputAction;
	class InputDevice;


	class DF2D_API InputActionResolver final : public IInputActions, public IInputFrameLifecycle, public IInputActionHandler
	{
	private:
		std::unordered_map<Data::InputUserID, ActionMapIndex> runtimeActionMaps;

		std::unordered_map<Data::InputUserID, std::unordered_set<RuntimeInputAction*>> activeActions;

		std::unordered_set<RuntimeInputAction*> callableActions;


		ActionPhase ResolvePhase(bool started, bool held, bool cancelled);

		void ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);

		void ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);

		void ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);


		int ToCustomCode(const InputDevice& device, Models::InputControlType inputControlType, int sdlCode);

		int ToSDLCode(const InputDevice& device, Models::InputControlType inputControlType, int customCode);


		void InputUserCreatedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);

		void InputUserDestroyedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);


		void BeginFrame() override;

		void PreUpdate() override;


		void ProcessBinding(const InputDevice& device, Models::InputControlType inputControlType, int controlID) override;


	public:
		InputActionResolver();

		~InputActionResolver() override;


		Utilities::ListenerID RegisterAction(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const Engine::ComponentHandleBase& listener,
			const std::function<void(const InputActionView&)>& handler) override;

		void DeregisterAction(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const Engine::ComponentHandleBase& listener) override;

		void DeregisterActionByID(
			Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			Utilities::ListenerID listenerID) override;

		bool EnableActionMap(Data::InputUserID userID, const std::string& name) override;

		bool DisableActionMap(Data::InputUserID userID, const std::string& name) override;

		bool SwitchToActionMap(Data::InputUserID userID, const std::string& name) override;

		std::optional<InputActionView> GetActionState(Data::InputUserID userID, const std::string actionName) override;
	};
}