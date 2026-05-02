#pragma once
#include <limits>
#include <numbers>


namespace DF2D::Constants::MathConstants
{
	inline constexpr double PI = std::numbers::pi;

	inline constexpr float PI_f = std::numbers::pi_v<float>;

	inline constexpr float EPSILON = std::numeric_limits<float>::epsilon();
}