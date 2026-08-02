#pragma once
#include <string>


namespace DF2D::Data
{
	enum class MessageBoxButtonFlags
	{
		NONE,

		/** @brief Button activated when the user presses Return. */
		RETURN_KEY_DEFAULT,

		/** @brief Button activated when the user presses Escape. */
		ESCAPE_KEY_DEFAULT
	};


	struct MessageBoxButton
	{
		/** @brief Value returned by the dialog when this button is picked. */
		int id = 0;

		std::string text = "";

		MessageBoxButtonFlags flags = MessageBoxButtonFlags::NONE;
	};
}