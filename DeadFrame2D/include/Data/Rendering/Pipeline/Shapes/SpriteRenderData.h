#pragma once
#include "Constants/CommonColors.h"
#include <SDL_render.h>


namespace DeadFrame2D::Data
{
	struct SpriteRenderData
	{
		SDL_Texture* texture = nullptr;

		const SDL_Rect* srcRect = NULL;

		const SDL_FRect* destRect = NULL;

		SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;

		float rotation = 0.0f;

		SDL_FPoint rotationOrigin = SDL_FPoint{ 0.0f, 0.0f };

		SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE;
	};
}