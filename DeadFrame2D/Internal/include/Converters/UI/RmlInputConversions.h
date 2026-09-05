#pragma once
#include "Data/Systems/UI/KeyModifiers.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"
#include "Models/Input/Controls/MouseButtonCode.h"
#include <RmlUi/Core/Input.h>


namespace DF2D::Internal::RmlInputConversions
{
	/** @brief Translates the engine's modifier flags into the bitmask the UI library expects. */
	inline int ToKeyModifierState(Data::KeyModifiers modifiers)
	{
		auto state = 0;

		if (Data::HasModifier(modifiers, Data::KeyModifiers::SHIFT))
			state |= Rml::Input::KM_SHIFT;

		if (Data::HasModifier(modifiers, Data::KeyModifiers::CTRL))
			state |= Rml::Input::KM_CTRL;

		if (Data::HasModifier(modifiers, Data::KeyModifiers::ALT))
			state |= Rml::Input::KM_ALT;

		if (Data::HasModifier(modifiers, Data::KeyModifiers::META))
			state |= Rml::Input::KM_META;

		return state;
	}

	/** @brief Button indices the UI library uses: left 0, right 1, middle 2. */
	inline int ToButtonIndex(Models::MouseButtonCode button)
	{
		switch (button)
		{
		case Models::MouseButtonCode::RIGHT:	return 1;
		case Models::MouseButtonCode::MIDDLE:	return 2;
		default:								return 0;
		}
	}

	/**
	 * @brief Translates an engine KeyboardKeyCode into the RmlUI library's key identifier.
	 *
	 * Only the keys text editing and menu navigation actually need are mapped; anything else resolves
	 * to UNKNOWN, which the UI ignores and therefore leaves for gameplay.
	 */
	inline Rml::Input::KeyIdentifier ToKeyIdentifier(Models::KeyboardKeyCode key)
	{
		using Key = Models::KeyboardKeyCode;

		switch (key)
		{
		case Key::A:				return Rml::Input::KI_A;
		case Key::B:				return Rml::Input::KI_B;
		case Key::C:				return Rml::Input::KI_C;
		case Key::D:				return Rml::Input::KI_D;
		case Key::E:				return Rml::Input::KI_E;
		case Key::F:				return Rml::Input::KI_F;
		case Key::G:				return Rml::Input::KI_G;
		case Key::H:				return Rml::Input::KI_H;
		case Key::I:				return Rml::Input::KI_I;
		case Key::J:				return Rml::Input::KI_J;
		case Key::K:				return Rml::Input::KI_K;
		case Key::L:				return Rml::Input::KI_L;
		case Key::M:				return Rml::Input::KI_M;
		case Key::N:				return Rml::Input::KI_N;
		case Key::O:				return Rml::Input::KI_O;
		case Key::P:				return Rml::Input::KI_P;
		case Key::Q:				return Rml::Input::KI_Q;
		case Key::R:				return Rml::Input::KI_R;
		case Key::S:				return Rml::Input::KI_S;
		case Key::T:				return Rml::Input::KI_T;
		case Key::U:				return Rml::Input::KI_U;
		case Key::V:				return Rml::Input::KI_V;
		case Key::W:				return Rml::Input::KI_W;
		case Key::X:				return Rml::Input::KI_X;
		case Key::Y:				return Rml::Input::KI_Y;
		case Key::Z:				return Rml::Input::KI_Z;

		case Key::NUM_0:			return Rml::Input::KI_0;
		case Key::NUM_1:			return Rml::Input::KI_1;
		case Key::NUM_2:			return Rml::Input::KI_2;
		case Key::NUM_3:			return Rml::Input::KI_3;
		case Key::NUM_4:			return Rml::Input::KI_4;
		case Key::NUM_5:			return Rml::Input::KI_5;
		case Key::NUM_6:			return Rml::Input::KI_6;
		case Key::NUM_7:			return Rml::Input::KI_7;
		case Key::NUM_8:			return Rml::Input::KI_8;
		case Key::NUM_9:			return Rml::Input::KI_9;

		case Key::ENTER:			return Rml::Input::KI_RETURN;
		case Key::ESCAPE:			return Rml::Input::KI_ESCAPE;
		case Key::BACKSPACE:		return Rml::Input::KI_BACK;
		case Key::TAB:				return Rml::Input::KI_TAB;
		case Key::SPACE:			return Rml::Input::KI_SPACE;

		case Key::MINUS:			return Rml::Input::KI_OEM_MINUS;
		case Key::EQUALS:			return Rml::Input::KI_OEM_PLUS;
		case Key::LEFT_BRACKET:		return Rml::Input::KI_OEM_4;
		case Key::RIGHT_BRACKET:	return Rml::Input::KI_OEM_6;
		case Key::BACKSLASH:		return Rml::Input::KI_OEM_5;
		case Key::SEMICOLON:		return Rml::Input::KI_OEM_1;
		case Key::APOSTROPHE:		return Rml::Input::KI_OEM_7;
		case Key::GRAVE:			return Rml::Input::KI_OEM_3;
		case Key::COMMA:			return Rml::Input::KI_OEM_COMMA;
		case Key::PERIOD:			return Rml::Input::KI_OEM_PERIOD;
		case Key::SLASH:			return Rml::Input::KI_OEM_2;

		case Key::INSERT:			return Rml::Input::KI_INSERT;
		case Key::HOME:				return Rml::Input::KI_HOME;
		case Key::PAGE_UP:			return Rml::Input::KI_PRIOR;
		case Key::DELETE:			return Rml::Input::KI_DELETE;
		case Key::END:				return Rml::Input::KI_END;
		case Key::PAGE_DOWN:		return Rml::Input::KI_NEXT;
		case Key::RIGHT:			return Rml::Input::KI_RIGHT;
		case Key::LEFT:				return Rml::Input::KI_LEFT;
		case Key::DOWN:				return Rml::Input::KI_DOWN;
		case Key::UP:				return Rml::Input::KI_UP;

		case Key::KP_ENTER:			return Rml::Input::KI_NUMPADENTER;
		case Key::KP_0:				return Rml::Input::KI_NUMPAD0;
		case Key::KP_1:				return Rml::Input::KI_NUMPAD1;
		case Key::KP_2:				return Rml::Input::KI_NUMPAD2;
		case Key::KP_3:				return Rml::Input::KI_NUMPAD3;
		case Key::KP_4:				return Rml::Input::KI_NUMPAD4;
		case Key::KP_5:				return Rml::Input::KI_NUMPAD5;
		case Key::KP_6:				return Rml::Input::KI_NUMPAD6;
		case Key::KP_7:				return Rml::Input::KI_NUMPAD7;
		case Key::KP_8:				return Rml::Input::KI_NUMPAD8;
		case Key::KP_9:				return Rml::Input::KI_NUMPAD9;

		case Key::LCTRL:			return Rml::Input::KI_LCONTROL;
		case Key::RCTRL:			return Rml::Input::KI_RCONTROL;
		case Key::LSHIFT:			return Rml::Input::KI_LSHIFT;
		case Key::RSHIFT:			return Rml::Input::KI_RSHIFT;
		case Key::LALT:				return Rml::Input::KI_LMENU;
		case Key::RALT:				return Rml::Input::KI_RMENU;

		case Key::F1:				return Rml::Input::KI_F1;
		case Key::F2:				return Rml::Input::KI_F2;
		case Key::F3:				return Rml::Input::KI_F3;
		case Key::F4:				return Rml::Input::KI_F4;
		case Key::F5:				return Rml::Input::KI_F5;
		case Key::F6:				return Rml::Input::KI_F6;
		case Key::F7:				return Rml::Input::KI_F7;
		case Key::F8:				return Rml::Input::KI_F8;
		case Key::F9:				return Rml::Input::KI_F9;
		case Key::F10:				return Rml::Input::KI_F10;
		case Key::F11:				return Rml::Input::KI_F11;
		case Key::F12:				return Rml::Input::KI_F12;

		default:					return Rml::Input::KI_UNKNOWN;
		}
	}
}