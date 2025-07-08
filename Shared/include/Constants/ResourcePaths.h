#pragma once
#include "Tools/FileSystem/ResolvedPath.h"
#include "Tools/FileSystem/ResourceMount.h"
#include <string_view>


namespace Constants::ResourcePaths
{
	namespace Folders
	{
		// Keys
		inline constexpr std::string_view SHARED_FONTS_ALIAS = "shared_fonts";

		inline constexpr std::string_view SHARED_INPUT_CONTROLS_ALIAS = "shared_input";

		inline constexpr std::string_view SHARED_ENGINE_CONFIG_ALIAS = "shared_engine_config";

		inline constexpr std::string_view LOGOS_ALIAS = "logos";


		inline static const ResourceMount SHARED_FONTS{std::string(SHARED_FONTS_ALIAS), "Shared/Resources/Fonts/" };

		inline static const ResourceMount SHARED_INPUT_CONTROLS{ std::string(SHARED_INPUT_CONTROLS_ALIAS), "Shared/Resources/Configurations/" };

		inline static const ResourceMount SHARED_ENGINE_CONFIG{std::string(SHARED_ENGINE_CONFIG_ALIAS), "Shared/Resources/Configurations/" };

		inline static const ResourceMount LOGOS{std::string(LOGOS_ALIAS), "Shared/Resources/Logos/" };
	}

	namespace Files
	{
		static const ResolvedPath CONSOLAS_FONT{ std::string(Folders::SHARED_FONTS_ALIAS), "consola.ttf" };

		static const ResolvedPath INPUT_CONTROLS{ std::string(Folders::SHARED_INPUT_CONTROLS_ALIAS), "InputControls.json" };

		static const ResolvedPath ENGINE_CONFIGURATION{ std::string(Folders::SHARED_ENGINE_CONFIG_ALIAS), "Manifest.json" };

		static const ResolvedPath WINDOW_ICON{ std::string(Folders::LOGOS_ALIAS), "Window_Icon.png" };

		static const ResolvedPath SPLASH_SCREEN{ std::string(Folders::LOGOS_ALIAS), "Splash_Screen.png" };
	}
}