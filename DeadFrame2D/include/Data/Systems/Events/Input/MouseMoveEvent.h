#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/UI/KeyModifiers.h"


namespace DF2D::Data
{
	struct MouseMoveEvent
	{
		Core::Vector2F position;

		Core::Vector2F delta;

		KeyModifiers modifiers = KeyModifiers::NONE;
	};
}