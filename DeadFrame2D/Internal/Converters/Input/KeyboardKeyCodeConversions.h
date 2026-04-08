#pragma once
#include <Models/Input/Controls/KeyboardKeyCode.h>
#include <SDL_scancode.h>


namespace DeadFrame2D::Internal::KeyboardKeyCodeConversions
{
	/**
	 * @brief Converts a KeyboardKeyCode to SDL's SDL_Scancode.
	 * @param code The custom keyboard key code.
	 * @return Corresponding SDL_Scancode.
	 */
	inline SDL_Scancode ToSDLScancode(Shared::Models::KeyboardKeyCode keyCode)
	{
		using namespace Shared::Models;

		switch (keyCode)
		{
		case KeyboardKeyCode::A: return SDL_SCANCODE_A;
		case KeyboardKeyCode::B: return SDL_SCANCODE_B;
		case KeyboardKeyCode::C: return SDL_SCANCODE_C;
		case KeyboardKeyCode::D: return SDL_SCANCODE_D;
		case KeyboardKeyCode::E: return SDL_SCANCODE_E;
		case KeyboardKeyCode::F: return SDL_SCANCODE_F;
		case KeyboardKeyCode::G: return SDL_SCANCODE_G;
		case KeyboardKeyCode::H: return SDL_SCANCODE_H;
		case KeyboardKeyCode::I: return SDL_SCANCODE_I;
		case KeyboardKeyCode::J: return SDL_SCANCODE_J;
		case KeyboardKeyCode::K: return SDL_SCANCODE_K;
		case KeyboardKeyCode::L: return SDL_SCANCODE_L;
		case KeyboardKeyCode::M: return SDL_SCANCODE_M;
		case KeyboardKeyCode::N: return SDL_SCANCODE_N;
		case KeyboardKeyCode::O: return SDL_SCANCODE_O;
		case KeyboardKeyCode::P: return SDL_SCANCODE_P;
		case KeyboardKeyCode::Q: return SDL_SCANCODE_Q;
		case KeyboardKeyCode::R: return SDL_SCANCODE_R;
		case KeyboardKeyCode::S: return SDL_SCANCODE_S;
		case KeyboardKeyCode::T: return SDL_SCANCODE_T;
		case KeyboardKeyCode::U: return SDL_SCANCODE_U;
		case KeyboardKeyCode::V: return SDL_SCANCODE_V;
		case KeyboardKeyCode::W: return SDL_SCANCODE_W;
		case KeyboardKeyCode::X: return SDL_SCANCODE_X;
		case KeyboardKeyCode::Y: return SDL_SCANCODE_Y;
		case KeyboardKeyCode::Z: return SDL_SCANCODE_Z;

		case KeyboardKeyCode::NUM_0: return SDL_SCANCODE_0;
		case KeyboardKeyCode::NUM_1: return SDL_SCANCODE_1;
		case KeyboardKeyCode::NUM_2: return SDL_SCANCODE_2;
		case KeyboardKeyCode::NUM_3: return SDL_SCANCODE_3;
		case KeyboardKeyCode::NUM_4: return SDL_SCANCODE_4;
		case KeyboardKeyCode::NUM_5: return SDL_SCANCODE_5;
		case KeyboardKeyCode::NUM_6: return SDL_SCANCODE_6;
		case KeyboardKeyCode::NUM_7: return SDL_SCANCODE_7;
		case KeyboardKeyCode::NUM_8: return SDL_SCANCODE_8;
		case KeyboardKeyCode::NUM_9: return SDL_SCANCODE_9;

		case KeyboardKeyCode::ENTER: return SDL_SCANCODE_RETURN;
		case KeyboardKeyCode::ESCAPE: return SDL_SCANCODE_ESCAPE;
		case KeyboardKeyCode::BACKSPACE: return SDL_SCANCODE_BACKSPACE;
		case KeyboardKeyCode::TAB: return SDL_SCANCODE_TAB;
		case KeyboardKeyCode::SPACE: return SDL_SCANCODE_SPACE;

		case KeyboardKeyCode::MINUS: return SDL_SCANCODE_MINUS;
		case KeyboardKeyCode::EQUALS: return SDL_SCANCODE_EQUALS;
		case KeyboardKeyCode::LEFT_BRACKET: return SDL_SCANCODE_LEFTBRACKET;
		case KeyboardKeyCode::RIGHT_BRACKET: return SDL_SCANCODE_RIGHTBRACKET;
		case KeyboardKeyCode::BACKSLASH: return SDL_SCANCODE_BACKSLASH;
		case KeyboardKeyCode::SEMICOLON: return SDL_SCANCODE_SEMICOLON;
		case KeyboardKeyCode::APOSTROPHE: return SDL_SCANCODE_APOSTROPHE;
		case KeyboardKeyCode::GRAVE: return SDL_SCANCODE_GRAVE;
		case KeyboardKeyCode::COMMA: return SDL_SCANCODE_COMMA;
		case KeyboardKeyCode::PERIOD: return SDL_SCANCODE_PERIOD;
		case KeyboardKeyCode::SLASH: return SDL_SCANCODE_SLASH;

		case KeyboardKeyCode::CAPS_LOCK: return SDL_SCANCODE_CAPSLOCK;

		case KeyboardKeyCode::F1: return SDL_SCANCODE_F1;
		case KeyboardKeyCode::F2: return SDL_SCANCODE_F2;
		case KeyboardKeyCode::F3: return SDL_SCANCODE_F3;
		case KeyboardKeyCode::F4: return SDL_SCANCODE_F4;
		case KeyboardKeyCode::F5: return SDL_SCANCODE_F5;
		case KeyboardKeyCode::F6: return SDL_SCANCODE_F6;
		case KeyboardKeyCode::F7: return SDL_SCANCODE_F7;
		case KeyboardKeyCode::F8: return SDL_SCANCODE_F8;
		case KeyboardKeyCode::F9: return SDL_SCANCODE_F9;
		case KeyboardKeyCode::F10: return SDL_SCANCODE_F10;
		case KeyboardKeyCode::F11: return SDL_SCANCODE_F11;
		case KeyboardKeyCode::F12: return SDL_SCANCODE_F12;
		case KeyboardKeyCode::F13: return SDL_SCANCODE_F13;
		case KeyboardKeyCode::F14: return SDL_SCANCODE_F14;
		case KeyboardKeyCode::F15: return SDL_SCANCODE_F15;
		case KeyboardKeyCode::F16: return SDL_SCANCODE_F16;
		case KeyboardKeyCode::F17: return SDL_SCANCODE_F17;
		case KeyboardKeyCode::F18: return SDL_SCANCODE_F18;
		case KeyboardKeyCode::F19: return SDL_SCANCODE_F19;
		case KeyboardKeyCode::F20: return SDL_SCANCODE_F20;
		case KeyboardKeyCode::F21: return SDL_SCANCODE_F21;
		case KeyboardKeyCode::F22: return SDL_SCANCODE_F22;
		case KeyboardKeyCode::F23: return SDL_SCANCODE_F23;
		case KeyboardKeyCode::F24: return SDL_SCANCODE_F24;

		case KeyboardKeyCode::PRINT_SCREEN: return SDL_SCANCODE_PRINTSCREEN;
		case KeyboardKeyCode::SCROLL_LOCK: return SDL_SCANCODE_SCROLLLOCK;
		case KeyboardKeyCode::PAUSE: return SDL_SCANCODE_PAUSE;
		case KeyboardKeyCode::INSERT: return SDL_SCANCODE_INSERT;
		case KeyboardKeyCode::HOME: return SDL_SCANCODE_HOME;
		case KeyboardKeyCode::PAGE_UP: return SDL_SCANCODE_PAGEUP;
		case KeyboardKeyCode::DELETE: return SDL_SCANCODE_DELETE;
		case KeyboardKeyCode::END: return SDL_SCANCODE_END;
		case KeyboardKeyCode::PAGE_DOWN: return SDL_SCANCODE_PAGEDOWN;
		case KeyboardKeyCode::RIGHT: return SDL_SCANCODE_RIGHT;
		case KeyboardKeyCode::LEFT: return SDL_SCANCODE_LEFT;
		case KeyboardKeyCode::DOWN: return SDL_SCANCODE_DOWN;
		case KeyboardKeyCode::UP: return SDL_SCANCODE_UP;

		case KeyboardKeyCode::NUM_LOCK: return SDL_SCANCODE_NUMLOCKCLEAR;
		case KeyboardKeyCode::KP_DIVIDE: return SDL_SCANCODE_KP_DIVIDE;
		case KeyboardKeyCode::KP_MULTIPLY: return SDL_SCANCODE_KP_MULTIPLY;
		case KeyboardKeyCode::KP_MINUS: return SDL_SCANCODE_KP_MINUS;
		case KeyboardKeyCode::KP_PLUS: return SDL_SCANCODE_KP_PLUS;
		case KeyboardKeyCode::KP_ENTER: return SDL_SCANCODE_KP_ENTER;
		case KeyboardKeyCode::KP_0: return SDL_SCANCODE_KP_0;
		case KeyboardKeyCode::KP_1: return SDL_SCANCODE_KP_1;
		case KeyboardKeyCode::KP_2: return SDL_SCANCODE_KP_2;
		case KeyboardKeyCode::KP_3: return SDL_SCANCODE_KP_3;
		case KeyboardKeyCode::KP_4: return SDL_SCANCODE_KP_4;
		case KeyboardKeyCode::KP_5: return SDL_SCANCODE_KP_5;
		case KeyboardKeyCode::KP_6: return SDL_SCANCODE_KP_6;
		case KeyboardKeyCode::KP_7: return SDL_SCANCODE_KP_7;
		case KeyboardKeyCode::KP_8: return SDL_SCANCODE_KP_8;
		case KeyboardKeyCode::KP_9: return SDL_SCANCODE_KP_9;
		case KeyboardKeyCode::KP_PERIOD: return SDL_SCANCODE_KP_PERIOD;

		case KeyboardKeyCode::LCTRL: return SDL_SCANCODE_LCTRL;
		case KeyboardKeyCode::LSHIFT: return SDL_SCANCODE_LSHIFT;
		case KeyboardKeyCode::LALT: return SDL_SCANCODE_LALT;
		case KeyboardKeyCode::LGUI: return SDL_SCANCODE_LGUI;
		case KeyboardKeyCode::RCTRL: return SDL_SCANCODE_RCTRL;
		case KeyboardKeyCode::RSHIFT: return SDL_SCANCODE_RSHIFT;
		case KeyboardKeyCode::RALT: return SDL_SCANCODE_RALT;
		case KeyboardKeyCode::RGUI: return SDL_SCANCODE_RGUI;

		case KeyboardKeyCode::VOLUME_UP: return SDL_SCANCODE_VOLUMEUP;
		case KeyboardKeyCode::VOLUME_DOWN: return SDL_SCANCODE_VOLUMEDOWN;
		case KeyboardKeyCode::MUTE: return SDL_SCANCODE_MUTE;
		case KeyboardKeyCode::MEDIA_PLAY: return SDL_SCANCODE_AUDIOPLAY;
		case KeyboardKeyCode::MEDIA_STOP: return SDL_SCANCODE_AUDIOSTOP;
		case KeyboardKeyCode::MEDIA_NEXT: return SDL_SCANCODE_AUDIONEXT;
		case KeyboardKeyCode::MEDIA_PREV: return SDL_SCANCODE_AUDIOPREV;

		case KeyboardKeyCode::APPLICATION: return SDL_SCANCODE_APPLICATION;
		case KeyboardKeyCode::POWER: return SDL_SCANCODE_POWER;
		case KeyboardKeyCode::SLEEP: return SDL_SCANCODE_SLEEP;

		default:
			return SDL_SCANCODE_UNKNOWN;
		}
	}

	/**
	 * @brief Converts SDL's SDL_Scancode to a KeyboardKeyCode.
	 * @param scancode The SDL scancode.
	 * @return Corresponding KeyboardKeyCode.
	 */
	inline Shared::Models::KeyboardKeyCode ToKeyboardKeyCode(SDL_Scancode scancode)
	{
		using namespace Shared::Models;
		switch (scancode)
		{
		case SDL_SCANCODE_A: return KeyboardKeyCode::A;
		case SDL_SCANCODE_B: return KeyboardKeyCode::B;
		case SDL_SCANCODE_C: return KeyboardKeyCode::C;
		case SDL_SCANCODE_D: return KeyboardKeyCode::D;
		case SDL_SCANCODE_E: return KeyboardKeyCode::E;
		case SDL_SCANCODE_F: return KeyboardKeyCode::F;
		case SDL_SCANCODE_G: return KeyboardKeyCode::G;
		case SDL_SCANCODE_H: return KeyboardKeyCode::H;
		case SDL_SCANCODE_I: return KeyboardKeyCode::I;
		case SDL_SCANCODE_J: return KeyboardKeyCode::J;
		case SDL_SCANCODE_K: return KeyboardKeyCode::K;
		case SDL_SCANCODE_L: return KeyboardKeyCode::L;
		case SDL_SCANCODE_M: return KeyboardKeyCode::M;
		case SDL_SCANCODE_N: return KeyboardKeyCode::N;
		case SDL_SCANCODE_O: return KeyboardKeyCode::O;
		case SDL_SCANCODE_P: return KeyboardKeyCode::P;
		case SDL_SCANCODE_Q: return KeyboardKeyCode::Q;
		case SDL_SCANCODE_R: return KeyboardKeyCode::R;
		case SDL_SCANCODE_S: return KeyboardKeyCode::S;
		case SDL_SCANCODE_T: return KeyboardKeyCode::T;
		case SDL_SCANCODE_U: return KeyboardKeyCode::U;
		case SDL_SCANCODE_V: return KeyboardKeyCode::V;
		case SDL_SCANCODE_W: return KeyboardKeyCode::W;
		case SDL_SCANCODE_X: return KeyboardKeyCode::X;
		case SDL_SCANCODE_Y: return KeyboardKeyCode::Y;
		case SDL_SCANCODE_Z: return KeyboardKeyCode::Z;

		case SDL_SCANCODE_0: return KeyboardKeyCode::NUM_0;
		case SDL_SCANCODE_1: return KeyboardKeyCode::NUM_1;
		case SDL_SCANCODE_2: return KeyboardKeyCode::NUM_2;
		case SDL_SCANCODE_3: return KeyboardKeyCode::NUM_3;
		case SDL_SCANCODE_4: return KeyboardKeyCode::NUM_4;
		case SDL_SCANCODE_5: return KeyboardKeyCode::NUM_5;
		case SDL_SCANCODE_6: return KeyboardKeyCode::NUM_6;
		case SDL_SCANCODE_7: return KeyboardKeyCode::NUM_7;
		case SDL_SCANCODE_8: return KeyboardKeyCode::NUM_8;
		case SDL_SCANCODE_9: return KeyboardKeyCode::NUM_9;

		case SDL_SCANCODE_RETURN: return KeyboardKeyCode::ENTER;
		case SDL_SCANCODE_ESCAPE: return KeyboardKeyCode::ESCAPE;
		case SDL_SCANCODE_BACKSPACE: return KeyboardKeyCode::BACKSPACE;
		case SDL_SCANCODE_TAB: return KeyboardKeyCode::TAB;
		case SDL_SCANCODE_SPACE: return KeyboardKeyCode::SPACE;

		case SDL_SCANCODE_MINUS: return KeyboardKeyCode::MINUS;
		case SDL_SCANCODE_EQUALS: return KeyboardKeyCode::EQUALS;
		case SDL_SCANCODE_LEFTBRACKET: return KeyboardKeyCode::LEFT_BRACKET;
		case SDL_SCANCODE_RIGHTBRACKET: return KeyboardKeyCode::RIGHT_BRACKET;
		case SDL_SCANCODE_BACKSLASH: return KeyboardKeyCode::BACKSLASH;
		case SDL_SCANCODE_SEMICOLON: return KeyboardKeyCode::SEMICOLON;
		case SDL_SCANCODE_APOSTROPHE: return KeyboardKeyCode::APOSTROPHE;
		case SDL_SCANCODE_GRAVE: return KeyboardKeyCode::GRAVE;
		case SDL_SCANCODE_COMMA: return KeyboardKeyCode::COMMA;
		case SDL_SCANCODE_PERIOD: return KeyboardKeyCode::PERIOD;
		case SDL_SCANCODE_SLASH: return KeyboardKeyCode::SLASH;

		case SDL_SCANCODE_CAPSLOCK: return KeyboardKeyCode::CAPS_LOCK;

		case SDL_SCANCODE_F1: return KeyboardKeyCode::F1;
		case SDL_SCANCODE_F2: return KeyboardKeyCode::F2;
		case SDL_SCANCODE_F3: return KeyboardKeyCode::F3;
		case SDL_SCANCODE_F4: return KeyboardKeyCode::F4;
		case SDL_SCANCODE_F5: return KeyboardKeyCode::F5;
		case SDL_SCANCODE_F6: return KeyboardKeyCode::F6;
		case SDL_SCANCODE_F7: return KeyboardKeyCode::F7;
		case SDL_SCANCODE_F8: return KeyboardKeyCode::F8;
		case SDL_SCANCODE_F9: return KeyboardKeyCode::F9;
		case SDL_SCANCODE_F10: return KeyboardKeyCode::F10;
		case SDL_SCANCODE_F11: return KeyboardKeyCode::F11;
		case SDL_SCANCODE_F12: return KeyboardKeyCode::F12;
		case SDL_SCANCODE_F13: return KeyboardKeyCode::F13;
		case SDL_SCANCODE_F14: return KeyboardKeyCode::F14;
		case SDL_SCANCODE_F15: return KeyboardKeyCode::F15;
		case SDL_SCANCODE_F16: return KeyboardKeyCode::F16;
		case SDL_SCANCODE_F17: return KeyboardKeyCode::F17;
		case SDL_SCANCODE_F18: return KeyboardKeyCode::F18;
		case SDL_SCANCODE_F19: return KeyboardKeyCode::F19;
		case SDL_SCANCODE_F20: return KeyboardKeyCode::F20;
		case SDL_SCANCODE_F21: return KeyboardKeyCode::F21;
		case SDL_SCANCODE_F22: return KeyboardKeyCode::F22;
		case SDL_SCANCODE_F23: return KeyboardKeyCode::F23;
		case SDL_SCANCODE_F24: return KeyboardKeyCode::F24;

		case SDL_SCANCODE_PRINTSCREEN: return KeyboardKeyCode::PRINT_SCREEN;
		case SDL_SCANCODE_SCROLLLOCK: return KeyboardKeyCode::SCROLL_LOCK;
		case SDL_SCANCODE_PAUSE: return KeyboardKeyCode::PAUSE;
		case SDL_SCANCODE_INSERT: return KeyboardKeyCode::INSERT;
		case SDL_SCANCODE_HOME: return KeyboardKeyCode::HOME;
		case SDL_SCANCODE_PAGEUP: return KeyboardKeyCode::PAGE_UP;
		case SDL_SCANCODE_DELETE: return KeyboardKeyCode::DELETE;
		case SDL_SCANCODE_END: return KeyboardKeyCode::END;
		case SDL_SCANCODE_PAGEDOWN: return KeyboardKeyCode::PAGE_DOWN;
		case SDL_SCANCODE_RIGHT: return KeyboardKeyCode::RIGHT;
		case SDL_SCANCODE_LEFT: return KeyboardKeyCode::LEFT;
		case SDL_SCANCODE_DOWN: return KeyboardKeyCode::DOWN;
		case SDL_SCANCODE_UP: return KeyboardKeyCode::UP;

		case SDL_SCANCODE_NUMLOCKCLEAR: return KeyboardKeyCode::NUM_LOCK;
		case SDL_SCANCODE_KP_DIVIDE: return KeyboardKeyCode::KP_DIVIDE;
		case SDL_SCANCODE_KP_MULTIPLY: return KeyboardKeyCode::KP_MULTIPLY;
		case SDL_SCANCODE_KP_MINUS: return KeyboardKeyCode::KP_MINUS;
		case SDL_SCANCODE_KP_PLUS: return KeyboardKeyCode::KP_PLUS;
		case SDL_SCANCODE_KP_ENTER: return KeyboardKeyCode::KP_ENTER;
		case SDL_SCANCODE_KP_0: return KeyboardKeyCode::KP_0;
		case SDL_SCANCODE_KP_1: return KeyboardKeyCode::KP_1;
		case SDL_SCANCODE_KP_2: return KeyboardKeyCode::KP_2;
		case SDL_SCANCODE_KP_3: return KeyboardKeyCode::KP_3;
		case SDL_SCANCODE_KP_4: return KeyboardKeyCode::KP_4;
		case SDL_SCANCODE_KP_5: return KeyboardKeyCode::KP_5;
		case SDL_SCANCODE_KP_6: return KeyboardKeyCode::KP_6;
		case SDL_SCANCODE_KP_7: return KeyboardKeyCode::KP_7;
		case SDL_SCANCODE_KP_8: return KeyboardKeyCode::KP_8;
		case SDL_SCANCODE_KP_9: return KeyboardKeyCode::KP_9;
		case SDL_SCANCODE_KP_PERIOD: return KeyboardKeyCode::KP_PERIOD;

		case SDL_SCANCODE_LCTRL: return KeyboardKeyCode::LCTRL;
		case SDL_SCANCODE_LSHIFT: return KeyboardKeyCode::LSHIFT;
		case SDL_SCANCODE_LALT: return KeyboardKeyCode::LALT;
		case SDL_SCANCODE_LGUI: return KeyboardKeyCode::LGUI;
		case SDL_SCANCODE_RCTRL: return KeyboardKeyCode::RCTRL;
		case SDL_SCANCODE_RSHIFT: return KeyboardKeyCode::RSHIFT;
		case SDL_SCANCODE_RALT: return KeyboardKeyCode::RALT;
		case SDL_SCANCODE_RGUI: return KeyboardKeyCode::RGUI;

		case SDL_SCANCODE_VOLUMEUP: return KeyboardKeyCode::VOLUME_UP;
		case SDL_SCANCODE_VOLUMEDOWN: return KeyboardKeyCode::VOLUME_DOWN;
		case SDL_SCANCODE_MUTE: return KeyboardKeyCode::MUTE;
		case SDL_SCANCODE_AUDIOPLAY: return KeyboardKeyCode::MEDIA_PLAY;
		case SDL_SCANCODE_AUDIOSTOP: return KeyboardKeyCode::MEDIA_STOP;
		case SDL_SCANCODE_AUDIONEXT: return KeyboardKeyCode::MEDIA_NEXT;
		case SDL_SCANCODE_AUDIOPREV: return KeyboardKeyCode::MEDIA_PREV;

		case SDL_SCANCODE_APPLICATION: return KeyboardKeyCode::APPLICATION;
		case SDL_SCANCODE_POWER: return KeyboardKeyCode::POWER;
		case SDL_SCANCODE_SLEEP: return KeyboardKeyCode::SLEEP;

		default:
			return KeyboardKeyCode::UNKNOWN;
		}
	}
}