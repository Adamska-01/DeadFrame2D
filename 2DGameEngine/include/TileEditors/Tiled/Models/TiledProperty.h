#pragma once
#include <string>
#include <variant>


struct TiledProperty
{
	std::string Name;

	std::string Type;

	std::variant<bool, float, int, std::string> Value;
};