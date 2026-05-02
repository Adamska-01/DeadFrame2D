#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include <SDL_render.h>


namespace DF2D::Data
{
	struct PointRenderData
	{
		Core::Vector2F pos = Core::Vector2F::Zero;

		SDL_Color color = Constants::CommonColors::WHITE;
	};
}