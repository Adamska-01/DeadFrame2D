#pragma once
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>


struct NamedBitflag
{
	std::string name;

	uint16_t value;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(NamedBitflag, name, value)
};