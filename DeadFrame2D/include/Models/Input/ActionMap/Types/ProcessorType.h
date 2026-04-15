#pragma once
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
{
	enum class ProcessorType
	{
		DEADZONE,

		SCALE,

		INVERT,

		NORMALIZE,

		CLAMP
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ProcessorType,
	{
		{ ProcessorType::DEADZONE, "Deadzone" },
		{ ProcessorType::SCALE, "Scale" },
		{ ProcessorType::INVERT, "Invert" },
		{ ProcessorType::NORMALIZE, "Normalize" },
		{ ProcessorType::CLAMP, "Clamp" }
	});
}
