#pragma once
#include <Models/Input/InputBinding.h>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


namespace DeadFrame2D::Core
{
	class InputControls
	{
	private:
		static std::unordered_map<std::string, std::vector<Shared::Models::InputBinding>> inputActions;


	public:
		static void Deserialize(std::string_view filePath);

		// TODO: Delete all this shit..

		static std::unordered_map<std::string, std::vector<Shared::Models::InputBinding>> GetAllActions();

		static void AddOrOverrideAction(const std::string& actionName, const std::vector<Shared::Models::InputBinding>& bindings);

		static bool RemoveAction(const std::string& actionName);

		static bool AddBinding(const std::string& actionName, const Shared::Models::InputBinding& binding);

		static bool RemoveBinding(const std::string& actionName, const Shared::Models::InputBinding& binding);

		static std::optional<std::vector<Shared::Models::InputBinding>> GetAction(const std::string& actionName);

		static void PrintActions();
	};
}