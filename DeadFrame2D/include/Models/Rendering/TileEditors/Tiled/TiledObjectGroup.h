#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include <vector>


namespace DF2D::Models
{
	struct TiledObjectGroup
	{
		std::string name;

		std::vector<Core::Vector2F> points;
	};
}