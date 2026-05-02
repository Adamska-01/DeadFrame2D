#pragma once
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	struct ClampProcessorSettings
	{
		float min = 0.0f;

		float max = 1.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ClampProcessorSettings, min, max);
	};
}