#pragma once
#include "Core/Math/Vector2.h"
#include "Models/Input/Controls/MouseButtonCode.h"


namespace DF2D::Data
{
	struct MouseButtonEvent
	{
		Models::MouseButtonCode button = Models::MouseButtonCode::UNKNOWN;

		bool pressed = false;

		Core::Vector2F position;
	};
}