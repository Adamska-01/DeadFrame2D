#pragma once


namespace DF2D::Data
{
	/**
	 * @brief Which component writes the size of a parent-driven element.
	 *
	 * Only one may write it.
	 */
	enum class LayoutSizeSource
	{
		/** @brief The rect transform writes the size, from its sizeDelta. */
		RECT_TRANSFORM,

		/** @brief A LayoutElement writes the size and the flex properties. The rect transform writes neither. */
		LAYOUT_ELEMENT
	};
}