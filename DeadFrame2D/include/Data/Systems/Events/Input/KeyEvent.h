#pragma once
#include "Models/Input/Controls/KeyboardKeyCode.h"


namespace DF2D::Data
{
	struct KeyEvent
	{
		Models::KeyboardKeyCode key = Models::KeyboardKeyCode::UNKNOWN;

		bool pressed = false;
	};
}