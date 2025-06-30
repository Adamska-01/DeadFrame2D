#pragma once
#include "Generic/InputBinding.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


class InputControls
{
private:
	static std::unordered_map<std::string, std::vector<InputBinding>> inputActions;


public:
	static void Deserialize(std::string_view filePath);

	// TODO: Delete all this shit..

	static std::unordered_map<std::string, std::vector<InputBinding>> GetAllActions();

	static void AddOrOverrideAction(const std::string& actionName, const std::vector<InputBinding>& bindings);

	static bool RemoveAction(const std::string& actionName);

	static bool AddBinding(const std::string& actionName, const InputBinding& binding);

	static bool RemoveBinding(const std::string& actionName, const InputBinding& binding);

	static std::optional<std::vector<InputBinding>> GetAction(const std::string& actionName);

	static void PrintActions();
};