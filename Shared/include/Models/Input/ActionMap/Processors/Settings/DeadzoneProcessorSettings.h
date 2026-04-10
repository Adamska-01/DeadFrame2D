#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct DeadzoneProcessorSettings
	{
		float threshold = 0.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeadzoneProcessorSettings, threshold);
	};
}