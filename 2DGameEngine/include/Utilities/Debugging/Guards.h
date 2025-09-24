#pragma once
#include <stdexcept>
#include <string>


namespace DeadFrame2D::Utilities
{
	// TODO: put this in a static class
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