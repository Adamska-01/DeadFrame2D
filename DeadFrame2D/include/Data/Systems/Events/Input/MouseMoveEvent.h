#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	struct MouseMoveEvent
	{
		Core::Vector2F position;

		Core::Vector2F delta;
	};
}