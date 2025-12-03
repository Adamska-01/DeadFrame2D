#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class ActionType
	{
		BUTTON,

		VALUE
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ActionType, 
	{
		{ ActionType::BUTTON, "Button" },
		{ ActionType::VALUE, "Value" }
	});
}