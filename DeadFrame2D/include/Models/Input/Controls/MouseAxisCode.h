#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	enum class MouseAxisCode : uint8_t
	{
		MOTION_X,

		MOTION_Y,

		WHEEL_X,

		WHEEL_Y,

		POSITION_X,

		POSITION_Y,

		COUNT_MAX
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(MouseAxisCode,
	{
		{ MouseAxisCode::MOTION_X, "MotionX"},
		{ MouseAxisCode::MOTION_Y, "MotionY" },
		{ MouseAxisCode::WHEEL_X, "WheelX" },
		{ MouseAxisCode::WHEEL_Y, "WheelY" },
		{ MouseAxisCode::POSITION_X, "PositionX" },
		{ MouseAxisCode::POSITION_Y, "PositionY" }
	});
}