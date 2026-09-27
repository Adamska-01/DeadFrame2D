#pragma once
#include "Core/Math/Color.h"


namespace DF2D::Data
{
	/**
	 * @brief What the parts of a slider are tinted in one interaction state.
	 */
	struct SliderStateColors
	{
		Core::Color background;

		Core::Color fill;

		Core::Color handle;
	};


	/**
	 * @brief The three states a slider is drawn in, in the order the player reaches them.
	 */
	struct SliderColors
	{
		SliderStateColors resting
		{
			.background = Core::Color{ 0x1a, 0x1a, 0x1f, 255 },
			.fill = Core::Color{ 0x55, 0x55, 0x5f, 255 },
			.handle = Core::Color{ 0x9a, 0x9a, 0xa4, 255 } 
		};

		SliderStateColors focused
		{
			.background = Core::Color{ 0x24, 0x24, 0x2b, 255 },
			.fill = Core::Color{ 0x9a, 0x9a, 0xa4, 255 },
			.handle = Core::Color{ 0xe8, 0xe8, 0xec, 255 }
		};

		SliderStateColors activated
		{
			.background = Core::Color{ 0x24, 0x24, 0x2b, 255 },
			.fill = Core::Color{ 0xc4, 0x88, 0x1f, 255 },
			.handle = Core::Color{ 0xe0, 0xa0, 0x30, 255 }
		};
	};
}