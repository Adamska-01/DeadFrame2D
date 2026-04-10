#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct ScaleProcessorSettings
	{
		float factor = 1.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScaleProcessorSettings, factor);
	};
}