#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class InteractionType
	{
		PRESS,

		HOLD,

		TAP,

		DOUBLETAP,

		CUSTOM
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(InteractionType,
	{
		{ InteractionType::PRESS, "Press" },
		{ InteractionType::HOLD, "Hold" },
		{ InteractionType::TAP, "Tap" },
		{ InteractionType::DOUBLETAP, "DoubleTap" },
		{ InteractionType::CUSTOM, "Custom" }
	});
}