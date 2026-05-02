#pragma once
#include <cstdint>


namespace DF2D::Data
{
	enum class ObjectEntryState : uint8_t
	{
		ALIVE,

		DESTRUCTION_PENDING,

		DEAD
	};
}