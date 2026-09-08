#pragma once
#include <cstdint>


namespace DF2D::Utilities
{
	/**
	 * @brief Identifies one registered listener.
	 *
	 * Signed, because a failed registration returns -1.
	 */
	using ListenerID = int64_t;
}