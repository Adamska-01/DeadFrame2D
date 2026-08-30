#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Data/Systems/Rendering/BlendMode.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>


namespace DF2D::Internal::RenderingConversions
{
	inline SDL_Color ToSDLColor(const Core::Color& c)
	{
		return SDL_Color{ c.r, c.g, c.b, c.a };
	}

	inline Core::Color ToColor(const SDL_Color& c)
	{
		return Core::Color{ c.r, c.g, c.b, c.a };
	}


	inline SDL_FRect ToSDLFRect(const Core::RectF& r)
	{
		return SDL_FRect{ r.x, r.y, r.w, r.h };
	}

	inline Core::RectF ToRectF(const SDL_FRect& r)
	{
		return Core::RectF{ r.x, r.y, r.w, r.h };
	}


	inline SDL_Rect ToSDLRect(const Core::RectI& r)
	{
		return SDL_Rect{ r.x, r.y, r.w, r.h };
	}

	inline Core::RectI ToRectI(const SDL_Rect& r)
	{
		return Core::RectI{ r.x, r.y, r.w, r.h };
	}


	inline SDL_RendererFlip ToSDLRenderFlip(Data::RenderFlip f)
	{
		switch (f)
		{
		case Data::RenderFlip::HORIZONTAL:	return SDL_FLIP_HORIZONTAL;
		case Data::RenderFlip::VERTICAL:	return SDL_FLIP_VERTICAL;
		default:							return SDL_FLIP_NONE;
		}
	}

	inline SDL_BlendMode ToSDLBlendMode(Data::BlendMode m)
	{
		switch (m)
		{
		case Data::BlendMode::NONE:		return SDL_BLENDMODE_NONE;

		case Data::BlendMode::PREMULTIPLIED_ALPHA:
			// Source color is already scaled by its own alpha, so it must be added as-is rather
			// than multiplied by alpha again. Destination alpha accumulates the same way as in
			// ADDITIVE so that drawing onto a transparent render target still raises its alpha.
			return SDL_ComposeCustomBlendMode(
				SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD,
				SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);

		case Data::BlendMode::ADDITIVE:
			// Stock SDL_BLENDMODE_ADD leaves destination alpha untouched (dstA = dstA), so
			// additive draws onto a transparent target (e.g. a camera's off-screen render
			// target, cleared to alpha 0) never raise that alpha above zero. When that target
			// is later alpha-composited onto the screen, those pixels are still read as fully
			// transparent and the additive content silently disappears despite correct RGB.
			// Compose the same color-add behaviour but also raise destination alpha wherever
			// something was actually drawn, so compositing can see it.
			return SDL_ComposeCustomBlendMode(
				SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
				SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);

		default:						return SDL_BLENDMODE_BLEND;
		}
	}

	inline Data::RenderFlip ToRenderFlip(SDL_RendererFlip f)
	{
		switch (f)
		{
		case SDL_FLIP_HORIZONTAL:	return Data::RenderFlip::HORIZONTAL;
		case SDL_FLIP_VERTICAL:		return Data::RenderFlip::VERTICAL;
		default:					return Data::RenderFlip::NONE;
		}
	}
}
