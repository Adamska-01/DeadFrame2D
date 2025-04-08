#pragma once
#include "Math/Vector2.h"
#include <string>
#include <vector>


struct TiledObjectGroup
{
	std::string Name;
	
	std::vector<Vector2F> Points;
};