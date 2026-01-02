#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include <SDL_render.h>


namespace DeadFrame2D::Data
{
	struct CircleRenderData
	{
		DeadFrame2D::Core::Vector2F center = DeadFrame2D::Core::Vector2F::Zero;

		float radius = 1.0f;

		bool filled = true;

		SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE;
	};
}