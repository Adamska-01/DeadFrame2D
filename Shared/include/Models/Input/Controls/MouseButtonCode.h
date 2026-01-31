#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class MouseButtonCode : uint8_t
	{
		LEFT,

		MIDDLE,

		RIGHT,

		X1,

		X2,

		UNKNOWN,

		COUNT_MAX
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ControllerAxisCode,
	{
		{ MouseButtonCode::LEFT, "Left"},
		{ MouseButtonCode::MIDDLE, "Middle"},
		{ MouseButtonCode::RIGHT, "Right" },
		{ MouseButtonCode::X1, "X1" },
		{ MouseButtonCode::X2, "X2" }
	});
}