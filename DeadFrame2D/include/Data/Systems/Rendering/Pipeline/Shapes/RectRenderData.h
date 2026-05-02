#pragma once
#include "Constants/CommonColors.h"
#include <SDL_render.h>


namespace DF2D::Data
{
	struct RectRenderData
	{
		bool filled = true;

		SDL_FRect destRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

		float rotation = 0.0f;

		SDL_Color color = Constants::CommonColors::WHITE;
	};
}