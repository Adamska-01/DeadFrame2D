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
		DISABLED,

		/** @brief Whether a two-state control is switched on. Its presence is the state, not its value. */
		CHECKED,

		/** @brief Lowest value a range accepts. */
		MIN,

		/** @brief Highest value a range accepts. */
		MAX,

		/** @brief Distance between the values a range can settle on. */
		STEP,

		/** @brief Whether a range runs across or down. */
		ORIENTATION,

		/** @brief Longest text an editable element accepts. */
		MAX_LENGTH,

		/** @brief Text shown by an editable element while it is empty. */
		PLACEHOLDER
	};
}