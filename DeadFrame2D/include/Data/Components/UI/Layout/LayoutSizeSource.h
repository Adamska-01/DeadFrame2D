#pragma once


namespace DF2D::Data
{
	/**
	 * @brief Which component decides how big a parent-driven element is.
	 *
	 * Two components can describe a size, and only one of them may write it. A RectTransform's
	 * sizeDelta is the ordinary answer; when a LayoutElement is present, is the deliberate
	 * override and takes the size and flex properties for itself.
	 */
	enum class LayoutSizeSource
	{
		/** @brief The rect transform's own sizeDelta, and a size that flex may not shrink. */
		RECT_TRANSFORM,

		/** @brief A LayoutElement on the same object, which the rect transform leaves alone. */
		LAYOUT_ELEMENT
	};
}