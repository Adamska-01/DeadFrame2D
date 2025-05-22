#pragma once
#include "Generic/Bobble/BobbleConnectionDirection.h"
#include <array>
#include <Math/Vector2.h>


namespace BobbleConstants
{
	constexpr size_t MAX_BOBBLE_NEIGHBOURS = 6;

	constexpr int MIN_BOBBLE_TO_SCORE = 3;

	constexpr std::array<BobbleConnectionDirection, 6> BOBBLE_CONNECTION_DIRECTIONS
	{
		BobbleConnectionDirection::TOP_RIGHT,
		BobbleConnectionDirection::RIGHT,
		BobbleConnectionDirection::BOTTOM_RIGHT,
		BobbleConnectionDirection::BOTTOM_LEFT,
		BobbleConnectionDirection::LEFT,
		BobbleConnectionDirection::TOP_LEFT
	};

	static const std::array<Vector2F, 6> BOBBLE_HEX_DIRECTIONS =
	{
		Vector2F(std::cos(MathConstants::PI / 3), -std::sin(MathConstants::PI / 3)),	// TOPRIGHT
		Vector2F(1, 0),																	// RIGHT
		Vector2F(std::cos(MathConstants::PI / 3), std::sin(MathConstants::PI / 3)),		// BOTTOMRIGHT
		Vector2F(-std::cos(MathConstants::PI / 3), std::sin(MathConstants::PI / 3)),	// BOTTOMLEFT
		Vector2F(-1, 0),																// LEFT
		Vector2F(-std::cos(MathConstants::PI / 3), -std::sin(MathConstants::PI / 3))	// TOPLEFT
	};
}