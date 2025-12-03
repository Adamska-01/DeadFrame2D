#pragma once
#include "Models/Input/ActionMap/Types/ProcessorType.h"
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct InputProcessor
	{
		ProcessorType kind = ProcessorType::CUSTOM;

		// arbitrary processor-specific parameters
		nlohmann::json params;
	};


	inline void to_json(nlohmann::json& j, const InputProcessor& p)
	{
		j = nlohmann::json{ {"kind", p.kind}, {"params", p.params} };
	}

	inline void from_json(const nlohmann::json& j, InputProcessor& p)
	{
		j.at("kind").get_to(p.kind);

		if (j.contains("params"))
		{
			p.params = j.at("params");
		}
	}
}