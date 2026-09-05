#pragma once
#include <string>


namespace DF2D::Data
{
	/**
	 * @brief Text composed by the platform.
	 * 
	 * Separate from KeyEvent because this is the composed result rather than a physical key: one event
	 * can carry several characters, and an IME may produce it from a sequence of keys that individually
	 * mean nothing. Only produced between IUIPlatform::StartTextInput and StopTextInput.
	 */
	struct TextInputEvent
	{
		/** @brief UTF-8 text to insert. */
		std::string text;
	};
}