#pragma once
#include "Models/Input/Controls/MouseButtonCode.h"
#include <SDL_mouse.h>


namespace DF2D::Internal::MouseButtonConversions
{
	/**
	 * @brief Converts a MouseButtonCode to SDL's internal mouse code.
	 * @param code The custom mouse button code.
	 * @return Corresponding SDL's internal mouse code.
	 */
	inline uint32_t ToSDLMouseButton(Models::MouseButtonCode mouseButtonCode)
	{
		using namespace Models;


		switch (mouseButtonCode)
		{
		case MouseButtonCode::LEFT:		return SDL_BUTTON_LEFT;
		case MouseButtonCode::MIDDLE:	return SDL_BUTTON_MIDDLE;
		case MouseButtonCode::RIGHT:	return SDL_BUTTON_RIGHT;
		case MouseButtonCode::X1:		return SDL_BUTTON_X1;
		case MouseButtonCode::X2:		return SDL_BUTTON_X2;

		default:
			return 0;
		}
	}

	/**
	 * @brief Converts SDL mouse code to a MouseButtonCode.
	 * @param sdlCode The SDL code.
	 * @return Corresponding MouseButtonCode.
	 */
	inline Models::MouseButtonCode ToMouseButtonCode(uint32_t sdlCode)
	{
		using namespace Models;


		switch (sdlCode)
		{
		case SDL_BUTTON_LEFT:		return MouseButtonCode::LEFT;
		case SDL_BUTTON_MIDDLE:		return MouseButtonCode::MIDDLE;
		case SDL_BUTTON_RIGHT:		return MouseButtonCode::RIGHT;
		case SDL_BUTTON_X1:			return MouseButtonCode::X1;
		case SDL_BUTTON_X2:			return MouseButtonCode::X2;

		default:
			return MouseButtonCode::UNKNOWN;
		}
	}
}