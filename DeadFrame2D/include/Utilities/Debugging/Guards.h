#pragma once
#include <stdexcept>
#include <string>


namespace DeadFrame2D::Utilities::Guard
{
	template<typename T>
	inline T AgainstNull(T ptr, const std::string& message)
	{
		if (ptr == nullptr)
		{
			throw std::invalid_argument(message);
		}

		return ptr;
	}
}