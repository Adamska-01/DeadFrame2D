#pragma once
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
{
	struct NormalizeProcessorSettings
	{
	};


	inline void to_json(nlohmann::json& j, const NormalizeProcessorSettings&)
	{
		j = nlohmann::json::object();
	}

	inline void from_json(const nlohmann::json&, NormalizeProcessorSettings&)
	{
	}
}