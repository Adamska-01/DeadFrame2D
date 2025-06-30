#pragma once
#include <nlohmann/json.hpp>
#include <cstdint>


enum class InputDeviceType
{
	KEYBOARD,

	MOUSE,

	CONTROLLER
};


// Serialization functions (nlohmann)
NLOHMANN_JSON_SERIALIZE_ENUM(InputDeviceType,
{
	{InputDeviceType::KEYBOARD, "KEYBOARD"},
	{InputDeviceType::MOUSE, "MOUSE"},
	{InputDeviceType::CONTROLLER, "CONTROLLER"}
})