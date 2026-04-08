#pragma once
#include "Models/Input/ActionMap/Properties/InputProcessor.h"
#include "Models/Input/InputBinding.h"
#include <nlohmann/json.hpp>
#include <vector>


namespace Shared::Models
{
	struct Binding
	{
		std::string name;

		std::string action;

		InputBinding input;


		std::vector<InputProcessor> processors;
	};


	inline void to_json(nlohmann::json& j, const Binding& b)
	{
		j["name"] = b.name;
		j["action"] = b.action;
		j["input"] = b.input;

		if (!b.processors.empty())
		{
			j["processors"] = b.processors;
		}
	}

	inline void from_json(const nlohmann::json& j, Binding& b)
	{
		j.at("name").get_to(b.name);
		j.at("action").get_to(b.action);
		j.at("input").get_to(b.input);

		if (j.contains("processors"))
		{
			b.processors = j.at("processors").get<std::vector<InputProcessor>>();
		}
	}
}