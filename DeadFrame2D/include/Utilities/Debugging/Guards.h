#pragma once
#include <stdexcept>
#include <string>


#define NAME_OF(ptr) #ptr


namespace DeadFrame2D::Utilities::Guard
{
	template<typename T>
	inline T AgainstNull(T ptr, const std::string& varName)
	{
		if (ptr == nullptr)
		{
			throw std::invalid_argument("Invalid null pointer: " + varName + " — ensure it is properly initialized");
		}

		return ptr;
	}
}