#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include <SDL_render.h>


namespace DeadFrame2D::Data
{
	struct PointRenderData
	{
		DeadFrame2D::Core::Vector2F pos = DeadFrame2D::Core::Vector2F::Zero;

		SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE;
	};
}