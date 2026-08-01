#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/**
	 * @brief Axis-aligned box shape definition, expressed in pixels.
	 */
	struct BoxShapeDefinition2D
	{
		Core::Vector2F halfExtents = Core::Vector2F(0.5f, 0.5f);

		Core::Vector2F center = Core::Vector2F(0.0f, 0.0f);

		float angle = 0.0f;
	};
}