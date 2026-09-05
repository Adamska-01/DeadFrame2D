#pragma once
#include "Data/Systems/UI/KeyModifiers.h"
#include <SDL_keycode.h>


namespace DF2D::Internal::KeyModifierConversions
{
	/**
	 * @brief Converts SDL's modifier bitmask into KeyModifiers.
	 */
	inline Data::KeyModifiers ToKeyModifiers(uint16_t sdlModifiers)
	{
		auto modifiers = Data::KeyModifiers::NONE;

		if (sdlModifiers & KMOD_SHIFT)
			modifiers |= Data::KeyModifiers::SHIFT;

		if (sdlModifiers & KMOD_CTRL)
			modifiers |= Data::KeyModifiers::CTRL;

		if (sdlModifiers & KMOD_ALT)
			modifiers |= Data::KeyModifiers::ALT;

		if (sdlModifiers & KMOD_GUI)
			modifiers |= Data::KeyModifiers::META;

		return modifiers;
	}
}