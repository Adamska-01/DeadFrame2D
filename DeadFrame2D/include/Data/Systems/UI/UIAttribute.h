#pragma once


namespace DF2D::Data
{
	/** @brief Non-style element attributes the component layer can set. */
	enum class UIAttribute
	{
		/** @brief Element identifier, used to target an element from navigation properties. */
		ID,

		/** @brief Image source path. */
		SOURCE,

		/** @brief Current value of an input-like element. */
		VALUE,

		/** @brief Whether an input-like element rejects interaction. */
		DISABLED
	};
}