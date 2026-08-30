#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/**
	 * @brief A single vertex of an arbitrary textured triangle mesh.
	 *
	 * The member order and packing are load-bearing: backends are allowed to hand the position,
	 * texture-coordinate and color channels straight to the graphics API as three strided views over
	 * an array of these, so reordering the members changes the wire format.
	 */
	struct Vertex2D
	{
		Core::Vector2F position;

		Core::Vector2F texCoord;

		Core::Color color;
	};
}