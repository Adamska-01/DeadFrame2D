#include "SubSystems/Input/InputControls.h"
#include <iostream>


std::unordered_map<std::string, std::vector<InputBinding>> InputControls::inputActions;


void InputControls::AddOrOverrideAction(const std::string& actionName, const std::vector<InputBinding>& bindings)
{
	inputActions[actionName] = bindings;
}

bool InputControls::RemoveAction(const std::string& actionName)
{
	return inputActions.erase(actionName);
}

bool InputControls::AddBinding(const std::string& actionName, const InputBinding& binding)
{
	auto it = inputActions.find(actionName);

	if (it == inputActions.end())
		return false;

	it->second.push_back(binding);

	return false;
}

bool InputControls::RemoveBinding(const std::string& actionName, const InputBinding& binding)
{
	auto it = inputActions.find(actionName);

	if (it == inputActions.end())
		return false;

	auto& bindings = it->second;
	auto bindingIt = std::find_if(bindings.begin(), bindings.end(), [&](const InputBinding& b)
		{
			return b.inputDeviceType == binding.inputDeviceType && b.inputKey == binding.inputKey;
		});

	if (bindingIt == bindings.end())
		return false;

	bindings.erase(bindingIt);

	return true;
}

std::optional<std::vector<InputBinding>> InputControls::GetAction(const std::string& actionName)
{
	auto it = inputActions.find(actionName);

	if (it != inputActions.end())
		return it->second;
	
	return std::nullopt;
}

void InputControls::PrintActions()
{
	for (const auto& [actionName, bindings] : inputActions)
	{
		std::cout << "Action: " << actionName << "\n";
		for (const auto& binding : bindings)
		{
			std::cout << "  - Device: " << (binding.inputDeviceType == InputDeviceType::KEYBOARD ? "Keyboard" : "Controller")
				<< ", Key: " << static_cast<int>(binding.inputKey) << "\n";
		}
	}
}