#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include <SDL_render.h>


namespace DF2D::Data
{
	struct CircleRenderData
	{
		Core::Vector2F center = Core::Vector2F::Zero;

		float radius = 1.0f;

		bool filled = true;

		SDL_Color color = Constants::CommonColors::WHITE;
	};
}