#pragma once
#include <cstdint>


namespace DF2D::Data
{
	// Values match TTF_STYLE_* so SDL backends can forward them directly.
	enum FontStyle
	{
		NORMAL			= 0x00,

		BOLD			= 0x01,

		ITALIC			= 0x02,

		UNDERLINE		= 0x04,

		STRIKETHROUGH	= 0x08
	};
}


namespace DF2D::Data
{
	inline FontStyle operator|(FontStyle lhs, FontStyle rhs)
	{
		return static_cast<FontStyle>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}
}