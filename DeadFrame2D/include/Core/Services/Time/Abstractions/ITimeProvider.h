#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Read-only view over the last completed frame's timings.
	 */
	class DF2D_API ITimeProvider
	{
	public:
		virtual ~ITimeProvider() = default;


		/**
		 * @brief Duration of the last frame in seconds, scaled by the current time scale.
		 */
		virtual float DeltaTime() const = 0;

		/**
		 * @brief Duration of the last frame in seconds, ignoring the current time scale.
		 */
		virtual float DeltaTimeUnscaled() const = 0;
	};
}