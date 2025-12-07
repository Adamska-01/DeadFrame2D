#pragma once
#include "Core/SubSystems/Systems/Input/Actions/ActionBindingLink.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include "Utilities/Hashing/TupleHash.h"
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


namespace DeadFrame2D::Core
{
	class RuntimeInputAction;
	class RuntimeActionMap;
	class InputDevice;


	class DF2D_API InputActionResolver
	{
	private:
		std::vector<std::shared_ptr<RuntimeActionMap>> runtimeActionMaps;

		std::unordered_map<
			std::tuple<std::string, Shared::Models::InputDeviceType, int>, 
			std::vector<ActionBindingLink>, 
			DeadFrame2D::Utilities::TupleHash> fastLookupActionMaps;

		std::unordered_set<RuntimeInputAction*> activeActions;


		ActionPhase ResolvePhase(bool started, bool held, bool cancelled);

		void ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);


	public:
		InputActionResolver();

		~InputActionResolver() = default;


		void BeginFrame();

		void ProcessBinding(const InputDevice& device, int controlID);

		void FinalizeActions();


		bool EnableActionMap(const std::string& name);

		bool DisableActionMap(const std::string& name);

		bool SwitchToActionMap(const std::string& name);

		std::optional<RuntimeInputAction> GetActionStateTEST(const std::string actionName);
	};
}