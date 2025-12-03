#pragma once
#include <nlohmann/json.hpp>
#include <cstdint>


namespace Shared::Models
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