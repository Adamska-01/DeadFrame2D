#pragma once
#include <SDL_render.h>


namespace DeadFrame2D::Data
{
	struct RenderState
	{
		float rotation = 0.0f;

		SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;

		SDL_Color color = SDL_Color{ 255, 255, 255, 255 };
	};
}