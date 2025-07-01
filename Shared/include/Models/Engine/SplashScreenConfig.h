#pragma once
#include <nlohmann/json.hpp>
#include <string_view>


struct SplashScreenConfig
{
	std::string_view imageSourcePath;

	int durationSeconds = 0;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SplashScreenConfig, imageSourcePath, durationSeconds)
};