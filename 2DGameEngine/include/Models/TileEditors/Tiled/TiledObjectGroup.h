#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include <vector>


namespace DeadFrame2D::Models
{
	struct TiledObjectGroup
	{
		std::string name;

		std::vector<DeadFrame2D::Core::Vector2F> points;
	};
}