#pragma once
#include "Models/Engine/RendererConfig.h"
#include "Models/Engine/SplashScreenConfig.h"
#include "Models/Engine/WindowConfig.h"
#include <nlohmann/json.hpp>


struct EngineConfig
{
	WindowConfig window;

	RendererConfig rendering;

	SplashScreenConfig splashScreen;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(EngineConfig, window, splashScreen)
};