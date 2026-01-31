#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class ControllerButtonCode : int
	{
		INVALID = -1,

		A,

		B,

		X,

		Y,

		BACK,

		GUIDE,

		START,

		LEFT_STICK,

		RIGHT_STICK,

		LEFT_SHOULDER,

		RIGHT_SHOULDER,

		DPAD_UP,

		DPAD_DOWN,

		DPAD_LEFT,

		DPAD_RIGHT,

		MISC1,		// Xbox Series X share, PS5 mic, Switch Pro capture, Luna mic

		PADDLE1,	// Xbox Elite paddle P1

		PADDLE2,	// Xbox Elite paddle P3

		PADDLE3,	// Xbox Elite paddle P2

		PADDLE4,	// Xbox Elite paddle P4

		TOUCHPAD,	// PS4/PS5 touchpad

		COUNT_MAX
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ControllerAxisCode,
	{
		{ ControllerButtonCode::A, "A"},
		{ ControllerButtonCode::B, "B" },
		{ ControllerButtonCode::X, "X" },
		{ ControllerButtonCode::Y, "Y" },
		{ ControllerButtonCode::BACK, "Back" },
		{ ControllerButtonCode::GUIDE, "Guide" },
		{ ControllerButtonCode::START, "Start" },
		{ ControllerButtonCode::LEFT_STICK, "LeftStick" },
		{ ControllerButtonCode::RIGHT_STICK, "RightStick" },
		{ ControllerButtonCode::LEFT_SHOULDER, "LeftShoulder" },
		{ ControllerButtonCode::RIGHT_SHOULDER, "RightShoulder" },
		{ ControllerButtonCode::DPAD_UP, "DpadUp" },
		{ ControllerButtonCode::DPAD_DOWN, "DpadDown" },
		{ ControllerButtonCode::DPAD_LEFT, "DpadLeft" },
		{ ControllerButtonCode::DPAD_RIGHT, "DpadRight" },
		{ ControllerButtonCode::MISC1, "MISC1" },
		{ ControllerButtonCode::PADDLE1, "Paddle1" },
		{ ControllerButtonCode::PADDLE2, "Paddle2" },
		{ ControllerButtonCode::PADDLE3, "Paddle3" },
		{ ControllerButtonCode::PADDLE4, "Paddle4" },
		{ ControllerButtonCode::TOUCHPAD, "TouchPad" }
	});
}