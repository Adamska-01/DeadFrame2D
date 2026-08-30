#pragma once
#include <cstdint>


namespace DF2D::Data
{
	/** @brief Modifier keys held while an input event was produced. */
	enum class KeyModifiers : uint8_t
	{
		NONE = 0,

		SHIFT = 1 << 0,

		CTRL = 1 << 1,

		ALT = 1 << 2,

		META = 1 << 3
	};


	inline constexpr KeyModifiers operator|(KeyModifiers a, KeyModifiers b)
	{
		return static_cast<KeyModifiers>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline constexpr KeyModifiers& operator|=(KeyModifiers& a, KeyModifiers b)
	{
		a = a | b;

		return a;
	}

	inline constexpr bool HasModifier(KeyModifiers value, KeyModifiers flag)
	{
		return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
	}
}