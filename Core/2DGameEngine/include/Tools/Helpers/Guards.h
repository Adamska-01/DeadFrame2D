#pragma once
#include <stdexcept>
#include <string>


namespace Tools::Helpers
{
	template<typename T>
	T GuardAgainstNull(T ptr, const std::string& message)
	{
		if (ptr == nullptr)
		{
			throw std::invalid_argument(message);
		}

		return ptr;
	}
}