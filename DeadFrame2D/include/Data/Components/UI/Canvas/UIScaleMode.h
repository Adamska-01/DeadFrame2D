#pragma once


namespace DF2D::Data
{
	/**
	 * @brief How a canvas's dp units convert to pixels as the render target's resolution changes.
	 */
	enum class UIScaleMode
	{
		/** @brief One dp is always one pixel; UI takes up more or less of the screen as it resizes. */
		CONSTANT_PIXEL_SIZE,

		/** 
		* @brief dp scales with the render target relative to a reference resolution, so UI keeps its
		* proportion of the screen across resolutions.
		*/
		SCALE_WITH_SCREEN_SIZE
	};
}