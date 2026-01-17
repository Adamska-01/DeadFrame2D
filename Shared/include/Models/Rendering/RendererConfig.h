#pragma once
#include <nlohmann/json.hpp>
#include <optional>


namespace Shared::Models
{
	struct RendererConfig
	{
		int width = 1920;

		int height = 1080;

		std::optional<int> targetFramerate;
	};


	inline void to_json(nlohmann::json& j, const RendererConfig& config)
	{
		j["width"] = config.width;
		j["height"] = config.height;

		if (config.targetFramerate)
		{
			j["targetFramerate"] = *config.targetFramerate;
		}
	}

	inline void from_json(const nlohmann::json& j, RendererConfig& config)
	{
		j.at("width").get_to(config.width);
		j.at("height").get_to(config.height);

		if (j.contains("targetFramerate"))
		{
			config.targetFramerate = j.at("targetFramerate").get<int>();
		}
	}
}