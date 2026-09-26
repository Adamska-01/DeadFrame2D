#pragma once


namespace DF2D::Data
{
	/**
	 * @brief What the focused widget wants done with a navigation input aimed at it.
	 */
	enum class UINavigationResponse
	{
		/** @brief Nothing special: the input moves focus, as it does for any other widget. */
		MOVE_FOCUS,

		/** @brief The focused widget takes the input and acts on it instead of focus moving. */
		CONSUME,

		/** @brief Neither: the input is swallowed, leaving focus and the widget as they are. */
		BLOCK
	};
}