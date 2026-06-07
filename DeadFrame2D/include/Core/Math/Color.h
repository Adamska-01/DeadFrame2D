#pragma once
#include <cstdint>


namespace DF2D::Core
{
	struct Color
	{
		uint8_t r = 0;

		uint8_t g = 0;

		uint8_t b = 0;

		uint8_t a = 255;


		constexpr Color() = default;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);


		constexpr bool operator==(const Color& other) const;

		constexpr bool operator!=(const Color& other) const;
	};
}


namespace DF2D::Core
{
	constexpr DF2D::Core::Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a)
	{
	}


	inline constexpr bool Color::operator==(const Color& other) const
	{
		return r == other.r
			&& g == other.g
			&& b == other.b
			&& a == other.a;
	}

	inline constexpr bool Color::operator!=(const Color& other) const
	{
		return !(*this == other);
	}
}