#pragma once


namespace DF2D::Data
{
	/**
	 * @brief How focus decides where to go next from a widget, for one direction.
	 *
	 * A direction may instead name a particular widget outright, which is what SetNavigation does.
	 */
	enum class UINavigationMode
	{
		/** @brief Focus does not leave this widget in that direction. */
		NONE,

		/** @brief The nearest widget that way on screen, worked out from where things ended up. */
		AUTOMATIC,

		/** @brief As AUTOMATIC, but only left and right lead anywhere. */
		HORIZONTAL,

		/** @brief As AUTOMATIC, but only up and down lead anywhere. */
		VERTICAL,

		/** @brief The next widget in the order they were added, regardless of where it sits. */
		TREE_ORDER
	};
}