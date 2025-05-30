#pragma once
#include "TiledProperty.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


struct TiledLayer
{
	std::string name;

	std::unordered_map<std::string, TiledProperty> Properties;

	std::vector<std::vector<int>> Data;


	std::optional<TiledProperty> GetProperty(std::string_view key) const
	{
		auto it = Properties.find(std::string(key));
		
		if (it != Properties.end()) 
			return it->second;

		return std::nullopt; 
	}

	float GetFloatProperty(std::string_view key, float defaultValue) const
	{
		auto prop = GetProperty(key);

		if (prop && std::holds_alternative<float>(prop->Value))
			return std::get<float>(prop->Value);
		
		return defaultValue;
	}

	bool GetBoolProperty(std::string_view key, bool defaultValue) const
	{
		auto prop = GetProperty(key);
		
		if (prop && std::holds_alternative<bool>(prop->Value))
			return std::get<bool>(prop->Value);
		
		return defaultValue;
	}
};