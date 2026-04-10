#include "Core/SubSystems/Systems/Input/Actions/RuntimeActionMap.h"
#include <unordered_map>
#include <unordered_set>


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;


	RuntimeActionMap::RuntimeActionMap(InputActionMap actionMap)
		: name(actionMap.name),
		enabled(false)
	{
		actions.clear();

		// Detect duplicate action names
		std::unordered_set<std::string> seenActionNames;

		for (auto& action : actionMap.actions)
		{
			if (!seenActionNames.insert(action.name).second) 
				throw std::runtime_error("Duplicate action name in InputActionMap: " + action.name);
		}

		// Build map: action name -> bindings
		std::unordered_map<std::string, std::vector<Binding>> nameToBindings;

		for (auto& binding : actionMap.bindings)
		{
			nameToBindings[binding.action].push_back(binding);
		}

		// Build runtime actions
		for (auto& action : actionMap.actions)
		{
			auto it = nameToBindings.find(action.name);

			if (it == nameToBindings.end())
			{
				throw std::runtime_error("Action has no bindings: " + action.name);
			}

			actions.emplace_back(
				action.name,
				action.valueType,
				it->second,
				action.processors);
		}
	}

	void RuntimeActionMap::Enable()
	{
		enabled = true;
	}

	void RuntimeActionMap::Disable()
	{
		enabled = false;
	}

	bool RuntimeActionMap::IsEnabled() const
	{
		return enabled;
	}

	const std::string& RuntimeActionMap::Name() const
	{
		return name;
	}

	std::vector<RuntimeInputAction>& RuntimeActionMap::GetActions()
	{
		return actions;
	}
}