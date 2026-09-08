#pragma once


namespace DF2D::Data
{
	/**
	* @brief The direction a grid fills before it wraps.
	*/
	enum class GridFlow
	{
		/** @brief Children fill a row across, then wrap onto the next row down. */
		ROWS,

		/** @brief Children fill a column downwards, then wrap onto the next column across. */
		COLUMNS
	};
}