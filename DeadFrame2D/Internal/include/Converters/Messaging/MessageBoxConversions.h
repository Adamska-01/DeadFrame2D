#pragma once
#include "Data/Services/Messaging/MessageBoxButton.h"
#include "Data/Services/Messaging/MessageBoxType.h"
#include <SDL.h>


namespace DF2D::Internal::MessageBoxConversions
{
	/**
	 * @brief Converts a MessageBoxType to SDL's message box flags.
	 * @param type The custom message box type.
	 * @return Corresponding SDL message box flags.
	 */
	inline SDL_MessageBoxFlags ToSDLMessageBoxFlags(Data::MessageBoxType type)
	{
		using namespace Data;


		switch (type)
		{
		case MessageBoxType::WARNING:	return SDL_MESSAGEBOX_WARNING;
		case MessageBoxType::CRITICAL:	return SDL_MESSAGEBOX_ERROR;

		default:
			return SDL_MESSAGEBOX_INFORMATION;
		}
	}

	/**
	 * @brief Converts button flags to SDL's message box button flags.
	 * @param flags The custom button flags.
	 * @return Corresponding SDL button flags.
	 */
	inline uint32_t ToSDLButtonFlags(Data::MessageBoxButtonFlags flags)
	{
		using namespace Data;


		switch (flags)
		{
		case MessageBoxButtonFlags::RETURN_KEY_DEFAULT:	return SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		case MessageBoxButtonFlags::ESCAPE_KEY_DEFAULT:	return SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;

		default:
			return 0;
		}
	}
}