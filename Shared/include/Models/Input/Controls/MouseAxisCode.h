#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class MouseAxisCode : uint8_t
	{
		MOTION_X,

		MOTION_Y,

		WHEEL_X,

		WHEEL_Y,

		COUNT_MAX
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ControllerAxisCode,
	{
		{ MouseAxisCode::MOTION_X, "MotionX"},
		{ MouseAxisCode::MOTION_Y, "MotionY" },
		{ MouseAxisCode::WHEEL_X, "WheelX" },
		{ MouseAxisCode::WHEEL_Y, "WheelY" }
	});
}