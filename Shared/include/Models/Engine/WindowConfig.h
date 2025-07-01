#pragma once
#include <nlohmann/json.hpp>
#include <string>


struct WindowConfig
{
	std::string title = "DeadFrame2D";

	std::string_view iconSourcePath;

	int width = 1920;

	int height = 1080;

	bool fullscreen = false;
	

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowConfig, title, iconSourcePath, width, height , fullscreen)
};