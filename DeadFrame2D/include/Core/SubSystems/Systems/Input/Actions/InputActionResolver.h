#pragma once
#include "Core/SubSystems/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/SubSystems/Systems/Input/Actions/Abstractions/IInputActions.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionBindingLink.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "Data/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Models/Input/InputDeviceType.h"
#include "Utilities/Hashing/TupleHash.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace Shared::Models
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
	class RuntimeActionMap;
	class InputDevice;


	class DF2D_API InputActionResolver final : public IInputActions, public IInputFrameLifecycle, public IInputActionHandler
	{
	private:
		std::unordered_map<DeadFrame2D::Data::InputUserID, std::vector<std::shared_ptr<RuntimeActionMap>>> runtimeActionMaps;

		std::unordered_map<
			DeadFrame2D::Data::InputUserID,
			std::unordered_map<
				std::tuple<std::string, Shared::Models::InputDeviceType, Shared::Models::InputControlType, int>,
				std::vector<ActionBindingLink>,
				DeadFrame2D::Utilities::TupleHash>> fastLookupActionMaps;

		std::unordered_map<DeadFrame2D::Data::InputUserID, std::unordered_set<RuntimeInputAction*>> activeActions;

		std::unordered_set<RuntimeInputAction*> callableActions;


		ActionPhase ResolvePhase(bool started, bool held, bool cancelled);

		void ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);


		int ToCustomCode(const InputDevice& device, Shared::Models::InputControlType inputControlType, int sdlCode);

		int ToSDLCode(const InputDevice& device, Shared::Models::InputControlType inputControlType, int customCode);


		void InputUserCreatedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);

		void InputUserDestroyedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);


		void BeginFrame() override;

		void PreUpdate() override;


		void ProcessBinding(const InputDevice& device, Shared::Models::InputControlType inputControlType, int controlID) override;


	public:
		InputActionResolver();

		~InputActionResolver() override;


		DeadFrame2D::Utilities::ListenerID RegisterAction(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener,
			const std::function<void(const RuntimeInputAction&)>& handler) override;

		void DeregisterAction(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			const DeadFrame2D::Engine::ComponentHandleBase& listener) override;

		void DeregisterActionByID(
			DeadFrame2D::Data::InputUserID userID,
			const std::string& actionMapName,
			const std::string& actionName,
			DeadFrame2D::Utilities::ListenerID listenerID) override;

		bool EnableActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) override;

		bool DisableActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) override;

		bool SwitchToActionMap(DeadFrame2D::Data::InputUserID userID, const std::string& name) override;

		std::optional<RuntimeInputAction> GetActionState(DeadFrame2D::Data::InputUserID userID, const std::string actionName) override;
	};
}