#pragma once


namespace DF2D::Data
{
	/** @brief Element events the backend forwards back into the component layer. */
	enum class UIEventType
	{
		CLICK,

		POINTER_ENTER,

		POINTER_EXIT,

		POINTER_DOWN,

		POINTER_UP,

		VALUE_CHANGED,

		SUBMIT,

		SCROLL,

		FOCUS_GAINED,

		FOCUS_LOST
	};
}