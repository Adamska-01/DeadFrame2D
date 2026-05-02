#pragma once


namespace DF2D::Data
{
	/**
	 * @brief Defines how a canvas is rendered relative to the screen or camera.
	 */
	enum class CanvasRenderMode
	{
		/**
		 * @brief Renders the canvas directly in screen space, relative to the window.
		 */
		SCREEN_SPACE_OVERLAY,

		/**
		 * @brief Renders the canvas in screen space but relative to a specific camera.
		 */
		SCREEN_SPACE_CAMERA
	};
}