#pragma once
#include <limits>
#include <numbers>


namespace DF2D::Constants::MathConstants
{
	inline constexpr double PI = std::numbers::pi;

	inline constexpr float PI_f = std::numbers::pi_v<float>;

	inline constexpr float EPSILON = std::numeric_limits<float>::epsilon();

	inline constexpr float RAD_PER_DEG = PI_f / 180.0f;

	inline constexpr float DEG_PER_RAD = 180.0f / PI_f;
}