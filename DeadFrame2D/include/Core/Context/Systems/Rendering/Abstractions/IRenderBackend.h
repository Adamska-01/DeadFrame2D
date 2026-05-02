#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include <SDL_render.h>


namespace DF2D::Core
{
	class DF2D_API IRenderBackend
	{
	public:
		virtual void DrawPixel(const Vector2F& pixelPos, SDL_Color color = Constants::CommonColors::WHITE) = 0;

		virtual void DrawLine(const Vector2F& p1, const Vector2F& p2, SDL_Color color = Constants::CommonColors::WHITE) = 0;

		virtual void DrawRect(SDL_FRect rect, float angleDegrees, SDL_Color color = Constants::CommonColors::WHITE, bool filled = false) = 0;

		virtual void DrawCircle(const Vector2F& center, float radius, SDL_Color color, bool filled) = 0;

		virtual void DrawTexture(
			SDL_Texture* texture,
			const SDL_Rect* srcRect = NULL,
			const SDL_FRect* dstRect = NULL,
			const SDL_FPoint* rotationOrigin = NULL,
			float angle = 0.0f,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			SDL_Color colorMod = Constants::CommonColors::WHITE) = 0;


		virtual void SetRenderTarget(SDL_Texture* renderTarget) = 0;

		virtual void ClearCurrentRenderTarget() = 0;
	};
}