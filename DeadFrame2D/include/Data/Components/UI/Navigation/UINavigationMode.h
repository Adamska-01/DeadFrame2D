#pragma once


namespace DF2D::Data
{
	/**
	 * @brief How focus decides where to go next from a widget.
	 *
	 * A widget may instead name its neighbours outright, which is what SetNavigationTarget does.
	 */
	enum class UINavigationMode
	{
		/** @brief Focus does not leave this widget in any direction. */
		NONE,

		/** @brief The nearest widget that way on screen, worked out from where things ended up. */
		AUTOMATIC,

		/** @brief As AUTOMATIC, but only left and right lead anywhere. */
		HORIZONTAL,

		/** @brief As AUTOMATIC, but only up and down lead anywhere. */
		VERTICAL,

		/** @brief The next widget in the order they were added, regardless of where it sits. */
		TREE_ORDER,

		/**
		 * @brief Only the neighbours named with SetNavigationTarget are reachable.
		 *
		 * A direction with no target set goes nowhere, which is what keeps a menu's edges from
		 * wandering into whatever happens to be laid out beyond them.
		 */
		EXPLICIT
	};
}