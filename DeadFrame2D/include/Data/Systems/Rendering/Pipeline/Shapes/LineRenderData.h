#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include <SDL_render.h>


namespace DF2D::Data
{
	struct LineRenderData
	{
		Core::Vector2F p1 = Core::Vector2F::Zero;

		Core::Vector2F p2 = Core::Vector2F::Zero;

		SDL_Color color = Constants::CommonColors::WHITE;
	};
}