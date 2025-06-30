#pragma once
#include "Math/Vector2.h"
#include <string>
#include <vector>


struct TiledObjectGroup
{
	std::string name;
	
	std::vector<Vector2F> points;
};