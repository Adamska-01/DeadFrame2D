#pragma once
#include <string>
#include <variant>


namespace DF2D::Models
{
	struct TiledProperty
	{
		std::string Name;

		std::string Type;

		std::variant<bool, float, int, std::string> Value;
	};
}