#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include <vector>


struct TiledObjectGroup
{
	std::string name;
	
	std::vector<Vector2F> points;
};