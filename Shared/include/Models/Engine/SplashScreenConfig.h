#pragma once
#include <nlohmann/json.hpp>
#include <string>


struct SplashScreenConfig
{
	float fadeInDurationSeconds = 1.0f;

	float holdVisibleDurationSeconds = 1.0f;
	
	float fadeOutDurationSeconds = 1.0f;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SplashScreenConfig, fadeInDurationSeconds, holdVisibleDurationSeconds, fadeOutDurationSeconds)
};