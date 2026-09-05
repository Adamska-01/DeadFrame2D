#pragma once
#include "Data/Systems/UI/CursorType.h"
#include <SDL_mouse.h>


namespace DF2D::Internal::CursorConversions
{
	/** @brief Maps the engine's cursor vocabulary onto SDL's system cursors. */
	inline SDL_SystemCursor ToSDLSystemCursor(Data::CursorType cursor)
	{
		switch (cursor)
		{
		case Data::CursorType::HAND:				return SDL_SYSTEM_CURSOR_HAND;
		case Data::CursorType::TEXT:				return SDL_SYSTEM_CURSOR_IBEAM;
		case Data::CursorType::MOVE:				return SDL_SYSTEM_CURSOR_SIZEALL;
		case Data::CursorType::RESIZE_HORIZONTAL:	return SDL_SYSTEM_CURSOR_SIZEWE;
		case Data::CursorType::RESIZE_VERTICAL:		return SDL_SYSTEM_CURSOR_SIZENS;
		case Data::CursorType::UNAVAILABLE:			return SDL_SYSTEM_CURSOR_NO;
		default:									return SDL_SYSTEM_CURSOR_ARROW;
		}
	}
}