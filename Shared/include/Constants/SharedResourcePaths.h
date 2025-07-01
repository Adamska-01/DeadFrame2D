#pragma once
#include <string_view>


namespace SharedResourcePaths
{
	namespace Fonts
	{
		inline constexpr std::string_view CONSOLAS_FONT_PATH = "Shared/Resources/Fonts/consola.ttf";
	}

	namespace Configurations
	{
		inline constexpr std::string_view INPUT_CONTROLS_CONFIGURATION_PATH = "Shared/Resources/Configurations/InputControls.json";

		inline constexpr std::string_view ENGINE_CONFIGURATION_PATH = "Shared/Resources/Configurations/Manifest.json";
	}
}