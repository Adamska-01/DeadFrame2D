#pragma once
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	struct DeadzoneProcessorSettings
	{
		float threshold = 0.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeadzoneProcessorSettings, threshold);
	};
}