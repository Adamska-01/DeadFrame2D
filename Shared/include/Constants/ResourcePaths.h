#pragma once
#include "Tools/FileSystem/ResolvedPath.h"
#include "Tools/FileSystem/ResourceMount.h"


namespace Constants::ResourcePaths
{
	namespace Folders
	{
		inline static const ResourceMount SHARED_FONTS{ "shared_fonts", "Shared/Resources/Fonts/" };
		
		inline static const ResourceMount SHARED_INPUT_CONTROLS{ "shared_input", "Shared/Resources/Configurations/" };
		
		inline static const ResourceMount SHARED_ENGINE_CONFIG{ "shared_engine_config", "Shared/Resources/Configurations/" };
		
		inline static const ResourceMount LOGOS{ "logos", "Shared/Resources/Logos/" };
	}

	namespace Files
	{
		static const ResolvedPath CONSOLAS_FONT = { Folders::SHARED_FONTS.alias, "consola.ttf" };

		static const ResolvedPath INPUT_CONTROLS = { Folders::SHARED_INPUT_CONTROLS.alias, "InputControls.json" };

		static const ResolvedPath ENGINE_CONFIGURATION = { Folders::SHARED_ENGINE_CONFIG.alias, "Manifest.json" };

		static const ResolvedPath WINDOW_ICON = { Folders::LOGOS.alias, "window_icon.png" };

		static const ResolvedPath SPLASH_SCREEN = { Folders::LOGOS.alias, "splash_screen.png" };
	}
}