#pragma once
#include "DF2D_API.h"
#include <chrono>


namespace DF2D::Core
{
	/**
	 * @brief Monotonic time source and sleep primitive used by the frame timer.
	 */
	class DF2D_API IClock
	{
	public:
		virtual ~IClock() = default;


		/**
		 * @brief Current value of a monotonic clock. Only differences are meaningful.
		 */
		virtual std::chrono::nanoseconds Now() const = 0;

		/**
		 * @brief Blocks the calling thread for at least the requested duration.
		 */
		virtual void SleepFor(std::chrono::nanoseconds duration) = 0;
	};
}