#pragma once
#include <stdexcept>
#include <string>


#define NAME_OF(ptr) #ptr


namespace DeadFrame2D::Utilities::Guard
{
	/**
	 * @brief Ensures that a pointer is not null.
	 * @tparam T Pointer or smart pointer type.
	 * @param ptr The pointer to check.
	 * @param varName The name of the variable being checked.
	 * @throws std::invalid_argument if ptr is null.
	 */
	template<typename T>
	inline void AgainstNull(const T& ptr, const std::string& varName)
	{
		if (ptr == nullptr)
		{
			throw std::invalid_argument("Invalid null pointer: " + varName + " — ensure it is properly initialized");
		}
	}

	/**
	 * @brief Ensures that a pointer is not null and returns it for assignment.
	 * @tparam T Pointer or smart pointer type.
	 * @param ptr The pointer to check and return.
	 * @param varName The name of the variable being assigned.
	 * @return The validated pointer (same as input).
	 * @throws std::invalid_argument if ptr is null.
	 */
	template<typename T>
	inline T AgainstNullAssignment(const T& ptr, const std::string& varName)
	{
		DeadFrame2D::Utilities::Guard::AgainstNull(ptr, varName);

		return ptr;
	}
}