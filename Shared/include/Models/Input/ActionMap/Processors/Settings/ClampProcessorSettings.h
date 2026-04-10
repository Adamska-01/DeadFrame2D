#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct ClampProcessorSettings
	{
		float min = 0.0f;

		float max = 1.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ClampProcessorSettings, min, max);
	};
}