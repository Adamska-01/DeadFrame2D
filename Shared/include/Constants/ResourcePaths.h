#pragma once
#include "Tools/FileSystem/ResolvedPath.h"
#include "Tools/FileSystem/ResourceMount.h"
#include <string_view>


namespace Shared::Constants::Paths
{
	namespace Folders
	{
		// Keys
		inline constexpr std::string_view SHARED_FONTS_ALIAS = "shared_fonts";

		inline constexpr std::string_view SHARED_INPUT_CONTROLS_ALIAS = "shared_input";

		inline constexpr std::string_view SHARED_ENGINE_CONFIG_ALIAS = "shared_engine_config";

		inline constexpr std::string_view SHARED_PHYSICS_CONFIGS_ALIAS = "shared_physics_config";

		inline constexpr std::string_view LOGOS_ALIAS = "logos";


		inline static const Shared::Tools::ResourceMount SHARED_FONTS{std::string(SHARED_FONTS_ALIAS), "Shared/Resources/Fonts/" };

		inline static const Shared::Tools::ResourceMount SHARED_INPUT_CONTROLS{ std::string(SHARED_INPUT_CONTROLS_ALIAS), "Shared/Resources/Configurations/Input/" };

		inline static const Shared::Tools::ResourceMount SHARED_ENGINE_CONFIG{std::string(SHARED_ENGINE_CONFIG_ALIAS), "Shared/Resources/Configurations/System/" };

		inline static const Shared::Tools::ResourceMount SHARED_PHYSICS_CONFIGS{std::string(SHARED_PHYSICS_CONFIGS_ALIAS), "Shared/Resources/Configurations/Physics/" };

		inline static const Shared::Tools::ResourceMount LOGOS{std::string(LOGOS_ALIAS), "Shared/Resources/Logos/" };
	}

	namespace Files
	{
		static const Shared::Tools::ResolvedPath CONSOLAS_FONT{ std::string(Folders::SHARED_FONTS_ALIAS), "consola.ttf" };

		static const Shared::Tools::ResolvedPath INPUT_CONTROLS{ std::string(Folders::SHARED_INPUT_CONTROLS_ALIAS), "InputControls.json" };

		static const Shared::Tools::ResolvedPath ENGINE_CONFIGURATION{ std::string(Folders::SHARED_ENGINE_CONFIG_ALIAS), "Manifest.json" };

		static const Shared::Tools::ResolvedPath COLLISION_MASKS{ std::string(Folders::SHARED_PHYSICS_CONFIGS_ALIAS), "CollisionMasks.json" };

		static const Shared::Tools::ResolvedPath WINDOW_ICON{ std::string(Folders::LOGOS_ALIAS), "Window_Icon.png" };

		static const Shared::Tools::ResolvedPath SPLASH_SCREEN{ std::string(Folders::LOGOS_ALIAS), "Splash_Screen.png" };
	}
}