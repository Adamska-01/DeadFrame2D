#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/**
	 * @brief Circle shape definition, expressed in pixels.
	 */
	struct CircleShapeDefinition2D
	{
		float radius = 0.5f;

		Core::Vector2F center = Core::Vector2F(0.0f, 0.0f);
	};
}