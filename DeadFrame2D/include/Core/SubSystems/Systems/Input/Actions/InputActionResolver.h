#pragma once
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include <memory>
#include <optional>
#include <string>
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
		std::vector<std::shared_ptr<RuntimeActionMap>> activeActionMaps;


		ActionPhase ResolvePhase(bool started, bool held, bool cancelled);

		void ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);

		void ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Shared::Models::Binding& binding);


	public:
		InputActionResolver();

		~InputActionResolver() = default;


		void ProcessAndSend(const std::vector<std::shared_ptr<InputDevice>>& allDevices);


		bool EnableActionMap(const std::string& name);

		bool DisableActionMap(const std::string& name);

		bool SwitchToActionMap(const std::string& name);

		std::optional<RuntimeInputAction> GetActionStateTEST(const std::string actionName);
	};
}