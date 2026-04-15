#pragma once
#include "Utilities/IO/FileSystem/ResolvedPath.h"
#include "Utilities/IO/FileSystem/ResourceMount.h"
#include <string_view>


namespace DeadFrame2D::Constants::Paths
{
	namespace Folders
	{
		// Keys
		inline constexpr std::string_view SHARED_FONTS_ALIAS = "shared_fonts";

		inline constexpr std::string_view SHARED_INPUT_CONTROLS_ALIAS = "shared_input";

		inline constexpr std::string_view SHARED_WINDOW_CONFIG_ALIAS = "shared_window_config";

		inline constexpr std::string_view SHARED_RENDER_CONFIG_ALIAS = "shared_render_config";

		inline constexpr std::string_view SHARED_AUDIO_CONFIG_ALIAS = "shared_audio_config";

		inline constexpr std::string_view SHARED_PHYSICS_CONFIG_ALIAS = "shared_physics_config";

		inline constexpr std::string_view SHARED_OTHER_CONFIG_ALIAS = "shared_other_config";

		inline constexpr std::string_view LOGOS_ALIAS = "logos";


		inline static const Utilities::ResourceMount SHARED_FONTS{std::string(SHARED_FONTS_ALIAS), "Resources/Fonts/" };

		inline static const Utilities::ResourceMount SHARED_INPUT_CONTROLS{ std::string(SHARED_INPUT_CONTROLS_ALIAS), "Resources/Configurations/Input/" };

		inline static const Utilities::ResourceMount SHARED_WINDOW_CONFIG{ std::string(SHARED_WINDOW_CONFIG_ALIAS), "Resources/Configurations/Window/" };

		inline static const Utilities::ResourceMount SHARED_RENDER_CONFIG{ std::string(SHARED_RENDER_CONFIG_ALIAS), "Resources/Configurations/Rendering/" };

		inline static const Utilities::ResourceMount SHARED_AUDIO_CONFIG{ std::string(SHARED_AUDIO_CONFIG_ALIAS), "Resources/Configurations/Audio/" };

		inline static const Utilities::ResourceMount SHARED_PHYSICS_CONFIGS{ std::string(SHARED_PHYSICS_CONFIG_ALIAS), "Resources/Configurations/Physics/" };

		inline static const Utilities::ResourceMount SHARED_OTHER_CONFIGS{ std::string(SHARED_OTHER_CONFIG_ALIAS), "Resources/Configurations/Other/" };

		inline static const Utilities::ResourceMount LOGOS{ std::string(LOGOS_ALIAS), "Resources/Logos/" };
	}

	namespace Files
	{
		static const Utilities::ResolvedPath CONSOLAS_FONT{ std::string(Folders::SHARED_FONTS_ALIAS), "consola.ttf" };


		static const Utilities::ResolvedPath WINDOW_ICON{ std::string(Folders::LOGOS_ALIAS), "Window_Icon.png" };

		static const Utilities::ResolvedPath SPLASH_SCREEN{ std::string(Folders::LOGOS_ALIAS), "Splash_Screen.png" };


		static const Utilities::ResolvedPath INPUT_CONTROLS{ std::string(Folders::SHARED_INPUT_CONTROLS_ALIAS), "InputControls.json" };

		static const Utilities::ResolvedPath WINDOW_CONFIGURATION{ std::string(Folders::SHARED_WINDOW_CONFIG_ALIAS), "WindowConfig.json" };

		static const Utilities::ResolvedPath RENDER_CONFIGURATION{ std::string(Folders::SHARED_RENDER_CONFIG_ALIAS), "RendererConfig.json" };

		static const Utilities::ResolvedPath AUDIO_CONFIGURATION{ std::string(Folders::SHARED_AUDIO_CONFIG_ALIAS), "AudioConfig.json" };
		
		static const Utilities::ResolvedPath PHYSICS_CONFIGURATION{ std::string(Folders::SHARED_PHYSICS_CONFIG_ALIAS), "PhysicsConfig.json" };

		static const Utilities::ResolvedPath COLLISION_MASKS{ std::string(Folders::SHARED_PHYSICS_CONFIG_ALIAS), "CollisionMasks.json" };

		static const Utilities::ResolvedPath SPLASH_SCREEN_CONFIGURATION{ std::string(Folders::SHARED_OTHER_CONFIG_ALIAS), "SplashScreenConfig.json" };
	}
}