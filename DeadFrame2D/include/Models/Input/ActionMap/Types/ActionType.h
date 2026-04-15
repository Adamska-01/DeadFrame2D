#pragma once
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
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