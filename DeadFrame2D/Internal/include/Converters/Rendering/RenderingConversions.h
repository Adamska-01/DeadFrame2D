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
		case Data::BlendMode::ADDITIVE:	return SDL_BLENDMODE_ADD;
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