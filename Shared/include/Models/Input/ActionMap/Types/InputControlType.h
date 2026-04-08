#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class InputControlType
	{
		ANALOG,

		DIGITAL
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(InputControlType,
	{
		{ InputControlType::ANALOG, "Analog" },
		{ InputControlType::DIGITAL, "Digital" }
	});
}