#pragma once
#include <string_view>


namespace AssetPaths
{
	namespace Maps
	{
		inline constexpr std::string_view SINGLEPLAYER_MAP_PATH = "App/Assets/Maps/SingleplayerMap.tmx";
		
		inline constexpr std::string_view MULTIPLAYER_MAP_PATH = "App/Assets/Maps/MultiplayerMap.tmx";
	}

	namespace Levels
	{
		inline constexpr std::string_view SINGLEPLAYER_LEVELS_FOLDER_PATH = "App/Assets/Levels";
	}

	namespace Backgrounds
	{
		inline constexpr std::string_view BACKGROUND_IMAGE_PATH = "App/Assets/Backgrounds/Stars.png";
	}

	namespace Sprites
	{
		inline constexpr std::string_view MAP_TILES_PATH = "App/Assets/Sprites/MapTiles.png";

		inline constexpr std::string_view TITLE_IMAGE_PATH = "App/Assets/Sprites/Title.png";

		inline constexpr std::string_view ARROW_IMAGE_PATH = "App/Assets/Sprites/Arrow.png";

		inline constexpr std::string_view IDLE_BOBBLE_PATH = "App/Assets/Sprites/IdleBobbles.png";

		inline constexpr std::string_view POP_BOBBLE_PATH = "App/Assets/Sprites/BlowBobbles.png";

		inline constexpr std::string_view BUTTON_IDLE_IMAGE_PATH = "App/Assets/Sprites/Buttons/Button_Idle.png";

		inline constexpr std::string_view BUTTON_HOVERED_IMAGE_PATH = "App/Assets/Sprites/Buttons/Button_Hovered.png";

		inline constexpr std::string_view BUTTON_PRESSED_IMAGE_PATH = "App/Assets/Sprites/Buttons/Button_Pressed.png";
	}

	namespace Fonts
	{
		inline constexpr std::string_view THE_BLAST_FONT_PATH = "App/Assets/Fonts/The Blast.otf";
	}
}