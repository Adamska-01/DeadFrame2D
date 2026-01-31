#pragma once
#include <cstdint>
#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class ControllerAxisCode : int8_t
	{
		INVALID = -1,

		LEFT_STICK_X,

		LEFT_STICK_Y,

		RIGHT_STICK_X,

		RIGHT_STICK_Y,

		LEFT_TRIGGER,

		RIGHT_TRIGGER,

		COUNT_MAX
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ControllerAxisCode,
	{
		{ ControllerAxisCode::LEFT_STICK_X, "LeftStickX" },
		{ ControllerAxisCode::LEFT_STICK_Y, "LeftStickY" },
		{ ControllerAxisCode::RIGHT_STICK_X, "RightStickX" },
		{ ControllerAxisCode::RIGHT_STICK_Y, "RightStickY" },
		{ ControllerAxisCode::LEFT_TRIGGER, "LeftTrigger" },
		{ ControllerAxisCode::RIGHT_TRIGGER, "RightTrigger" }
	});
}