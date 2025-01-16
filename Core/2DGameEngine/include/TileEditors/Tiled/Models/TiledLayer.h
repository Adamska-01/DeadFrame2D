#pragma once
#include "TiledProperty.h"
#include <string>
#include <unordered_map>
#include <vector>


struct TiledLayer
{
	std::unordered_map<std::string, TiledProperty> Properties;

	std::vector<std::vector<int>> Data;
};