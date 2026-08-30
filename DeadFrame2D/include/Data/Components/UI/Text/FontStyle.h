#pragma once
#include <cstdint>


namespace DF2D::Data
{
	/**
	 * @brief Typographic styles that can be combined on a run of text.
	 */
	enum class FontStyle : uint8_t
	{
		NORMAL			= 0,

		BOLD			= 1 << 0,

		ITALIC			= 1 << 1,

		UNDERLINE		= 1 << 2,

		STRIKETHROUGH	= 1 << 3
	};


	inline constexpr FontStyle operator|(FontStyle lhs, FontStyle rhs)
	{
		return static_cast<FontStyle>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline constexpr bool HasFontStyle(FontStyle value, FontStyle flag)
	{
		return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
	}
}
