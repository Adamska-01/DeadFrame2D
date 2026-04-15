#pragma once
#include "Models/Input/ActionMap/Processors/InputProcessor.h"
#include "Models/Input/ActionMap/Types/ActionType.h"
#include "Models/Input/ActionMap/Types/ValueType.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>


namespace DeadFrame2D::Models
{
	struct InputAction
	{
		std::string name;

		ActionType type = ActionType::BUTTON;

		ValueType valueType = ValueType::ANY;


		std::vector<InputProcessor> processors;
	};


	inline void to_json(nlohmann::json& j, const InputAction& a)
	{
		j["name"] = a.name;
		j["type"] = a.type;
		j["valueType"] = a.valueType;

		if (!a.processors.empty())
		{
			j["processors"] = a.processors;
		}
	}

	inline void from_json(const nlohmann::json& j, InputAction& a)
	{
		j.at("name").get_to(a.name);
		j.at("type").get_to(a.type);
		j.at("valueType").get_to(a.valueType);

		if (j.contains("processors"))
		{
			a.processors = j.at("processors").get<std::vector<InputProcessor>>();
		}
	}
}