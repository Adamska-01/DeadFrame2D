#pragma once


namespace DF2D::Data
{
	/**
	 * @brief Who decides where a UI element sits: the element itself, or the parent laying it out.
	 */
	enum class LayoutMode
	{
		/**
		 * @brief The element places itself against its parent's box using the anchor model.
		 *
		 * The default, and what a UI object under a plain container does.
		 */
		SELF_POSITIONED,

		/**
		 * @brief The parent places the element, and the anchor model gives way to it.
		 *
		 * A layout group arranges its children itself, so anchors and anchored positions have nothing
		 * left to say. The size the element asks for still counts; where it lands does not.
		 */
		PARENT_DRIVEN
	};
}