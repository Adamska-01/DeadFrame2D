#pragma once
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	struct InvertProcessorSettings
	{
	};


	inline void to_json(nlohmann::json& j, const InvertProcessorSettings&)
	{
		j = nlohmann::json::object();
	}

	inline void from_json(const nlohmann::json&, InvertProcessorSettings&)
	{
	}
}