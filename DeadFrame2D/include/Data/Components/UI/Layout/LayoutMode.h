#pragma once


namespace DF2D::Data
{
	/**
	 * @brief Whether a UI element places itself, or its parent places it.
	 */
	enum class LayoutMode
	{
		/** @brief The element places itself with the anchor model. The default. */
		SELF_POSITIONED,

		/**
		 * @brief A layout group places the element, so its anchors are ignored.
		 *
		 * Its size still applies.
		 */
		PARENT_DRIVEN
	};
}