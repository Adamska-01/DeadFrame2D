#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/**
	 * @brief Transform state of a physics body, expressed in engine units (pixels, radians).
	 */
	struct BodyTransform2D
	{
		Core::Vector2F position = Core::Vector2F(0.0f, 0.0f);

		float angle = 0.0f;
	};
}