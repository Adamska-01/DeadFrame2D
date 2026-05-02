#pragma once
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	struct NamedBitflag
	{
		std::string name = "NONE";

		uint16_t value = 0;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(NamedBitflag, name, value)
	};
}