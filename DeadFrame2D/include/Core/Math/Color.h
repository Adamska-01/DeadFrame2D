#pragma once
#include "Core/Math/MathUtils.h"
#include <cstdint>


namespace DF2D::Core
{
	struct Color
	{
		uint8_t r = 0;

		uint8_t g = 0;

		uint8_t b = 0;

		uint8_t a = 255;


		constexpr bool operator==(const Color& other) const;

		constexpr bool operator!=(const Color& other) const;


		/** @brief Linearly interpolates every channel, including alpha. t is clamped to [0, 1]. */
		static constexpr Color Lerp(const Color& a, const Color& b, float t);


	private:
		static constexpr uint8_t LerpChannel(uint8_t from, uint8_t to, float t);
	};
}


namespace DF2D::Core
{
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

	inline constexpr uint8_t Color::LerpChannel(uint8_t from, uint8_t to, float t)
	{
		const auto value = MathUtils::Lerp(static_cast<float>(from), static_cast<float>(to), t);

		return static_cast<uint8_t>(value + 0.5f);
	}

	inline constexpr Color Color::Lerp(const Color& a, const Color& b, float t)
	{
		const auto clampedT = MathUtils::Clamp(t, 0.0f, 1.0f);

		return Color
		{
			.r = LerpChannel(a.r, b.r, clampedT),
			.g = LerpChannel(a.g, b.g, clampedT),
			.b = LerpChannel(a.b, b.b, clampedT),
			.a = LerpChannel(a.a, b.a, clampedT)
		};
	}
}
