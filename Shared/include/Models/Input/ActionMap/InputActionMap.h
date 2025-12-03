#pragma once
#include "Models/Input/ActionMap/Bindings/Binding.h"
#include "Models/Input/ActionMap/InputAction.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>


namespace Shared::Models
{
	struct InputActionMap
	{
		std::string name;

		std::vector<InputAction> actions;

		std::vector<Binding> bindings;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(InputActionMap, name, actions, bindings)
	};
}