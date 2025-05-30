#pragma once
#include <array>
#include <string_view>


namespace Constants::GameMapLayerNames
{
	inline constexpr std::array<std::string_view, 2> RENDER_LAYER_NAMES
	{
		"Base",
		"Player_Stand"
	};

	inline constexpr std::array<std::string_view, 1> SIDE_WALLS_LAYER_NAME
	{
		"Side_Walls"
	};
	
	inline constexpr std::array<std::string_view, 1> TOP_WALLS_LAYER_NAME
	{
		"Top_Wall"
	};
}