#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
{
	enum class InputDeviceType
	{
		KEYBOARD,

		MOUSE,

		CONTROLLER
	};


	// Serialization functions (nlohmann)
	NLOHMANN_JSON_SERIALIZE_ENUM(InputDeviceType,
	{
		{InputDeviceType::KEYBOARD, "Keyboard"},
		{InputDeviceType::MOUSE, "Mouse"},
		{InputDeviceType::CONTROLLER, "Controller"}
	})
}