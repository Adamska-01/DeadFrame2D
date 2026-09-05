#pragma once
#include "Data/Systems/UI/KeyModifiers.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"


namespace DF2D::Data
{
	struct KeyEvent
	{
		Models::KeyboardKeyCode key = Models::KeyboardKeyCode::UNKNOWN;

		bool pressed = false;

		KeyModifiers modifiers = KeyModifiers::NONE;

		/** @brief Whether this is an auto-repeat rather than a fresh press. */
		bool repeat = false;
	};
}