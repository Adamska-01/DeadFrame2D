#pragma once
#include "Constants/CommonColors.h"
#include <optional>
#include <SDL_render.h>


namespace DF2D::Data
{
	struct SpriteRenderData
	{
		SDL_Texture* texture = nullptr;

		std::optional<SDL_Rect> srcRect = std::nullopt;

		std::optional<SDL_FRect> destRect = std::nullopt;

		SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;

		float rotation = 0.0f;

		std::optional<SDL_FPoint> rotationOrigin = std::nullopt;

		SDL_Color colorMod = Constants::CommonColors::WHITE;
	};
}