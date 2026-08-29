#pragma once


namespace DF2D::Core::MathUtils
{
	/** @brief Linearly interpolates between two values. Not clamped. */
	inline constexpr float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	/** @brief Constrains a value to the inclusive range formed by min and max. */
	inline constexpr float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;

		if (value > max)
			return max;

		return value;
	}
}