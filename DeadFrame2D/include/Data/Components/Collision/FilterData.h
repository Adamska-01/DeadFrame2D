#pragma once
#include <cstdint>


namespace DF2D::Data
{
	struct FilterData
	{
		uint16_t categoryBits = 0x0001;

		uint16_t maskBits = 0xFFFF;

		int16_t groupIndex = 0;
	};
}