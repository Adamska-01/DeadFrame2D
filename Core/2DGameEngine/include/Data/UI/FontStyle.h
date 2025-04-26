#pragma once
#include <SDL_ttf.h>


enum FontStyle
{
	NORMAL			= TTF_STYLE_NORMAL,
	
	BOLD			= TTF_STYLE_BOLD,
	
	ITALIC			= TTF_STYLE_ITALIC,
	
	UNDERLINE		= TTF_STYLE_UNDERLINE,
	
	STRIKETHROUGH	= TTF_STYLE_STRIKETHROUGH
};


inline FontStyle operator|(FontStyle lhs, FontStyle rhs)
{
	return static_cast<FontStyle>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}