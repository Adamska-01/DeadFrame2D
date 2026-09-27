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
			.background = Core::Color{ 0x2a, 0x2a, 0x33, 255 },
			.fill = Core::Color{ 0x7c, 0x7c, 0x8a, 255 },
			.handle = Core::Color{ 0xc8, 0xc8, 0xd2, 255 }
		};

		SliderStateColors focused
		{
			.background = Core::Color{ 0x33, 0x33, 0x3d, 255 },
			.fill = Core::Color{ 0xbc, 0xbc, 0xc6, 255 },
			.handle = Core::Color{ 0xff, 0xff, 0xff, 255 }
		};

		SliderStateColors activated
		{
			.background = Core::Color{ 0x33, 0x33, 0x3d, 255 },
			.fill = Core::Color{ 0xe0, 0xa0, 0x30, 255 },
			.handle = Core::Color{ 0xff, 0xd2, 0x7a, 255 }
		};
	};
}