#pragma once
#include "TiledProperty.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


struct TiledLayer
{
	std::unordered_map<std::string, TiledProperty> Properties;

	std::vector<std::vector<int>> Data;


	std::optional<TiledProperty> GetProperty(const std::string& key) const 
	{
		auto it = Properties.find(key);
		
		if (it != Properties.end()) 
			return it->second;

		return std::nullopt; 
	}
};