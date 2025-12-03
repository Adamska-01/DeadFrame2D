#pragma once
#include "Models/Input/ActionMap/Types/InteractionType.h"
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct InputInteraction
	{
		InteractionType kind = InteractionType::PRESS;

		nlohmann::json params;
	};


	inline void to_json(nlohmann::json& j, const InputInteraction& i)
	{
		j = nlohmann::json{ {"kind", i.kind}, {"params", i.params} };
	}

	inline void from_json(const nlohmann::json& j, InputInteraction& i)
	{
		j.at("kind").get_to(i.kind);

		if (j.contains("params"))
		{
			i.params = j.at("params");
		}
	}
}