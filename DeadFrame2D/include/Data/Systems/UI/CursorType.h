#pragma once


namespace DF2D::Data
{
	/** @brief Mouse cursor shapes the UI can ask the platform for. */
	enum class CursorType
	{
		ARROW,

		HAND,

		TEXT,

		MOVE,

		RESIZE_HORIZONTAL,

		RESIZE_VERTICAL,

		UNAVAILABLE
	};
}