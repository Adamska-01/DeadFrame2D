#pragma once
#include <cstdint>


namespace DF2D::Utilities
{
	/**
	 * @brief Identifies one registered listener.
	 *
	 * Signed, because a failed registration returns -1 and several places already say so. Valid ids
	 * come from a counter that starts at zero and only climbs, so the negative half of the range
	 * carries no meaning beyond "this did not work".
	 */
	using ListenerID = int64_t;
}