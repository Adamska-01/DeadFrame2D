#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputCaptureState.h"
#include "Core/Context/Systems/Input/Abstractions/IInputFrameLifecycle.h"
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActionHandler.h"
#include "Core/Context/Systems/Input/Actions/Abstractions/IInputActions.h"
#include "Core/Context/Systems/Input/Actions/ActionMapIndex.h"
#include "Core/Context/Systems/Input/Actions/ActionPhase.h"
#include "Core/Context/Systems/Input/User/Abstractions/IUserDevicePairings.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>


namespace DF2D::Models
{
	struct Binding;
}

namespace DF2D::Engine
{
	class ComponentHandleBase;
}


namespace DF2D::Core
{
	class RuntimeInputAction;
	class InputDevice;


	class DF2D_API InputActionResolver final : public IInputActions, public IInputActionHandler, public IInputFrameLifecycle
	{
	private:
		Models::InputActionMapBucket actionMapBucket;

		const IUserDevicePairings* userPairings;

		/**
		 * @brief Tells the resolver when the UI is claiming an input. Null means nothing ever claims.
		 */
		const IInputCaptureState* captureState = nullptr;

		std::unordered_map<Data::InputUserID, ActionMapIndex> runtimeActionMaps;

		std::unordered_map<Data::InputUserID, std::unordered_set<RuntimeInputAction*>> activeActions;

		std::unordered_set<RuntimeInputAction*> callableActions;


		/** @brief Whether the UI has claimed the kind of input this device produces. */
		bool IsCapturedBy(const InputDevice& device) const;

		ActionPhase ResolvePhase(bool started, bool held, bool cancelled);

		void ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);

		void ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);

		void ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Models::Binding& binding);


		void BeginFrame() override;

		void PreUpdate() override;


		void ProcessBinding(const InputDevice& device, Models::InputControlType inputControlType, int controlID) override;


	public:
		InputActionResolver(
			Models::InputActionMapBucket actionMapBucket,
			const IUserDevicePairings& userPairings,
			const IInputCaptureState* captureState);

		~InputActionResolver() override = default;


		void AddUser(Data::InputUserID userID);

		void RemoveUser(Data::InputUserID userID);


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