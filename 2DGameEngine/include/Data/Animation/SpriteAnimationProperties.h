#pragma once
#include <SDL.h>
#include <string>


namespace DeadFrame2D::Data
{
	/** @brief Configuration data for a sprite animation. */
	struct SpriteAnimationProperties
	{
		/** @brief Unique name of the animation. */
		std::string name;

		/** @brief Unique name of the animation. */
		std::string_view spriteSource;

		/** @brief Number of columns (frames) in the sprite sheet row. */
		int columnCount = 1;
	
		/** @brief Total number of rows in the sprite sheet. */
		int rowCount = 1;
	
		/** @brief Row of the sprite sheet to play. */
		int sourceRowNumber = 0;
	
		/** @brief Animation speed in frames per second. */
		float animSpeed = 1.0f;
	
		/** @brief Whether to loop the animation. */
		bool loop = false;
	};
}