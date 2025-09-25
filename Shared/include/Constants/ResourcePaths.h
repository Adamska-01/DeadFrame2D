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

		inline constexpr std::string_view SHARED_WINDOW_CONFIG_ALIAS = "shared_window_config";

		inline constexpr std::string_view SHARED_RENDER_CONFIG_ALIAS = "shared_render_config";

		inline constexpr std::string_view SHARED_AUDIO_CONFIG_ALIAS = "shared_audio_config";

		inline constexpr std::string_view SHARED_PHYSICS_CONFIG_ALIAS = "shared_physics_config";

		inline constexpr std::string_view SHARED_OTHER_CONFIG_ALIAS = "shared_other_config";

		inline constexpr std::string_view LOGOS_ALIAS = "logos";


		inline static const Shared::Tools::ResourceMount SHARED_FONTS{std::string(SHARED_FONTS_ALIAS), "Shared/Resources/Fonts/" };

		inline static const Shared::Tools::ResourceMount SHARED_INPUT_CONTROLS{ std::string(SHARED_INPUT_CONTROLS_ALIAS), "Shared/Resources/Configurations/Input/" };

		inline static const Shared::Tools::ResourceMount SHARED_WINDOW_CONFIG{ std::string(SHARED_WINDOW_CONFIG_ALIAS), "Shared/Resources/Configurations/Window/" };

		inline static const Shared::Tools::ResourceMount SHARED_RENDER_CONFIG{ std::string(SHARED_RENDER_CONFIG_ALIAS), "Shared/Resources/Configurations/Rendering/" };

		inline static const Shared::Tools::ResourceMount SHARED_AUDIO_CONFIG{ std::string(SHARED_AUDIO_CONFIG_ALIAS), "Shared/Resources/Configurations/Audio/" };

		inline static const Shared::Tools::ResourceMount SHARED_PHYSICS_CONFIGS{ std::string(SHARED_PHYSICS_CONFIG_ALIAS), "Shared/Resources/Configurations/Physics/" };

		inline static const Shared::Tools::ResourceMount SHARED_OTHER_CONFIGS{ std::string(SHARED_OTHER_CONFIG_ALIAS), "Shared/Resources/Configurations/Other/" };

		inline static const Shared::Tools::ResourceMount LOGOS{ std::string(LOGOS_ALIAS), "Shared/Resources/Logos/" };
	}

	namespace Files
	{
		static const Shared::Tools::ResolvedPath CONSOLAS_FONT{ std::string(Folders::SHARED_FONTS_ALIAS), "consola.ttf" };


		static const Shared::Tools::ResolvedPath WINDOW_ICON{ std::string(Folders::LOGOS_ALIAS), "Window_Icon.png" };

		static const Shared::Tools::ResolvedPath SPLASH_SCREEN{ std::string(Folders::LOGOS_ALIAS), "Splash_Screen.png" };


		static const Shared::Tools::ResolvedPath INPUT_CONTROLS{ std::string(Folders::SHARED_INPUT_CONTROLS_ALIAS), "InputControls.json" };

		static const Shared::Tools::ResolvedPath WINDOW_CONFIGURATION{ std::string(Folders::SHARED_WINDOW_CONFIG_ALIAS), "WindowConfig.json" };

		static const Shared::Tools::ResolvedPath RENDER_CONFIGURATION{ std::string(Folders::SHARED_RENDER_CONFIG_ALIAS), "RendererConfig.json" };

		static const Shared::Tools::ResolvedPath AUDIO_CONFIGURATION{ std::string(Folders::SHARED_AUDIO_CONFIG_ALIAS), "AudioConfig.json" };
		
		static const Shared::Tools::ResolvedPath PHYSICS_CONFIGURATION{ std::string(Folders::SHARED_PHYSICS_CONFIG_ALIAS), "PhysicsConfig.json" };

		static const Shared::Tools::ResolvedPath COLLISION_MASKS{ std::string(Folders::SHARED_PHYSICS_CONFIG_ALIAS), "CollisionMasks.json" };

		static const Shared::Tools::ResolvedPath SPLASH_SCREEN_CONFIGURATION{ std::string(Folders::SHARED_OTHER_CONFIG_ALIAS), "SplashScreenConfig.json" };

	}
}