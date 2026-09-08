#pragma once


namespace DF2D::Data
{
	/**
	* @brief How an axis of a UI element is sized.
	*/
	enum class SizeFitMode
	{
		/** @brief The size the element was given stands. */
		UNCONSTRAINED,

		/** @brief The element shrinks to whatever its content needs. */
		FIT_CONTENT
	};
}