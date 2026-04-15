#pragma once
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
{
	struct ScaleProcessorSettings
	{
		float factor = 1.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScaleProcessorSettings, factor);
	};
}