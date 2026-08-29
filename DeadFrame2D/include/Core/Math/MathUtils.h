#pragma once
#include "Constants/MathConstants.h"


namespace DF2D::Core::MathUtils
{
	/** @brief Converts an angle from degrees to radians. */
	inline constexpr float ToRadians(float degrees)
	{
		return degrees * Constants::MathConstants::RAD_PER_DEG;
	}

	/** @brief Converts an angle from radians to degrees. */
	inline constexpr float ToDegrees(float radians)
	{
		return radians * Constants::MathConstants::DEG_PER_RAD;
	}


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