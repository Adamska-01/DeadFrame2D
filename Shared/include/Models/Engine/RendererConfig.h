#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct RendererConfig
	{
		int width = 1920;

		int height = 1080;

		int targetFramerate = 60;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(RendererConfig, width, height, targetFramerate)
	};
}