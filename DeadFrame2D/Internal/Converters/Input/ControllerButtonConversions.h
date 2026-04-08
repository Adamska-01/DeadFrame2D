#pragma once
#include <Models/Input/Controls/ControllerAxisCode.h>
#include <Models/Input/Controls/ControllerButtonCode.h>
#include <SDL_gamecontroller.h>


namespace DeadFrame2D::Internal::ControllerButtonConversions
{
	/**
	 * @brief Converts a ControllerButtonCode to SDL's SDL_GameControllerButton.
	 * @param code The custom controller button code.
	 * @return Corresponding SDL_GameControllerButton.
	 */
	inline SDL_GameControllerButton ToSDLControllerButton(Shared::Models::ControllerButtonCode controllerButtonCode)
	{
		using namespace Shared::Models;


		switch (controllerButtonCode)
		{
		case ControllerButtonCode::INVALID:			return SDL_CONTROLLER_BUTTON_INVALID;

		case ControllerButtonCode::A:				return SDL_CONTROLLER_BUTTON_A;
		case ControllerButtonCode::B:				return SDL_CONTROLLER_BUTTON_B;
		case ControllerButtonCode::X:				return SDL_CONTROLLER_BUTTON_X;
		case ControllerButtonCode::Y:				return SDL_CONTROLLER_BUTTON_Y;
		case ControllerButtonCode::BACK:			return SDL_CONTROLLER_BUTTON_BACK;
		case ControllerButtonCode::GUIDE:			return SDL_CONTROLLER_BUTTON_GUIDE;
		case ControllerButtonCode::START:			return SDL_CONTROLLER_BUTTON_START;
		case ControllerButtonCode::LEFT_STICK:		return SDL_CONTROLLER_BUTTON_LEFTSTICK;
		case ControllerButtonCode::RIGHT_STICK:		return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
		case ControllerButtonCode::LEFT_SHOULDER:	return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
		case ControllerButtonCode::RIGHT_SHOULDER:	return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
		case ControllerButtonCode::DPAD_UP:			return SDL_CONTROLLER_BUTTON_DPAD_UP;
		case ControllerButtonCode::DPAD_DOWN:		return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
		case ControllerButtonCode::DPAD_LEFT:		return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
		case ControllerButtonCode::DPAD_RIGHT:		return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
		case ControllerButtonCode::MISC1:			return SDL_CONTROLLER_BUTTON_MISC1;
		case ControllerButtonCode::PADDLE1:			return SDL_CONTROLLER_BUTTON_PADDLE1;
		case ControllerButtonCode::PADDLE2:			return SDL_CONTROLLER_BUTTON_PADDLE2;
		case ControllerButtonCode::PADDLE3:			return SDL_CONTROLLER_BUTTON_PADDLE3;
		case ControllerButtonCode::PADDLE4:			return SDL_CONTROLLER_BUTTON_PADDLE4;
		case ControllerButtonCode::TOUCHPAD:		return SDL_CONTROLLER_BUTTON_TOUCHPAD;
		case ControllerButtonCode::COUNT_MAX:		return SDL_CONTROLLER_BUTTON_MAX;

		default:
			return SDL_CONTROLLER_BUTTON_INVALID;
		}
	}

	/**
	 * @brief Converts a ControllerButtonCode to SDL's SDL_GameControllerButton.
	 * @param code The custom controller button code.
	 * @return Corresponding SDL_GameControllerButton.
	 */
	inline SDL_GameControllerAxis ToSDLControllerAxis(Shared::Models::ControllerAxisCode axis)
	{
		using namespace Shared::Models;


		switch (axis)
		{
		case ControllerAxisCode::INVALID:		return SDL_CONTROLLER_AXIS_INVALID;

		case ControllerAxisCode::LEFT_STICK_X:		return SDL_CONTROLLER_AXIS_LEFTX;
		case ControllerAxisCode::LEFT_STICK_Y:		return SDL_CONTROLLER_AXIS_LEFTY;
		case ControllerAxisCode::RIGHT_STICK_X:		return SDL_CONTROLLER_AXIS_RIGHTX;
		case ControllerAxisCode::RIGHT_STICK_Y:		return SDL_CONTROLLER_AXIS_RIGHTY;
		case ControllerAxisCode::LEFT_TRIGGER:	return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
		case ControllerAxisCode::RIGHT_TRIGGER:	return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
		case ControllerAxisCode::COUNT_MAX:		return SDL_CONTROLLER_AXIS_MAX;

		default:
			return SDL_CONTROLLER_AXIS_INVALID;
		}
	}


	/**
	 * @brief Converts SDL controller button code to a ControllerButtonCode.
	 * @param sdlButtonCode The SDL controller button code.
	 * @return Corresponding ControllerButtonCode.
	 */
	inline Shared::Models::ControllerButtonCode ToControllerButtonCode(SDL_GameControllerButton sdlButtonCode)
	{
		using namespace Shared::Models;


		switch (sdlButtonCode)
		{
		case SDL_CONTROLLER_BUTTON_INVALID:			return ControllerButtonCode::INVALID;

		case SDL_CONTROLLER_BUTTON_A:				return ControllerButtonCode::A;
		case SDL_CONTROLLER_BUTTON_B:				return ControllerButtonCode::B;
		case SDL_CONTROLLER_BUTTON_X:				return ControllerButtonCode::X;
		case SDL_CONTROLLER_BUTTON_Y:				return ControllerButtonCode::Y;
		case SDL_CONTROLLER_BUTTON_BACK:			return ControllerButtonCode::BACK;
		case SDL_CONTROLLER_BUTTON_GUIDE:			return ControllerButtonCode::GUIDE;
		case SDL_CONTROLLER_BUTTON_START:			return ControllerButtonCode::START;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:		return ControllerButtonCode::LEFT_STICK;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:		return ControllerButtonCode::RIGHT_STICK;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:	return ControllerButtonCode::LEFT_SHOULDER;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:	return ControllerButtonCode::RIGHT_SHOULDER;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:			return ControllerButtonCode::DPAD_UP;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:		return ControllerButtonCode::DPAD_DOWN;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:		return ControllerButtonCode::DPAD_LEFT;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:		return ControllerButtonCode::DPAD_RIGHT;
		case SDL_CONTROLLER_BUTTON_MISC1:			return ControllerButtonCode::MISC1;
		case SDL_CONTROLLER_BUTTON_PADDLE1:			return ControllerButtonCode::PADDLE1;
		case SDL_CONTROLLER_BUTTON_PADDLE2:			return ControllerButtonCode::PADDLE2;
		case SDL_CONTROLLER_BUTTON_PADDLE3:			return ControllerButtonCode::PADDLE3;
		case SDL_CONTROLLER_BUTTON_PADDLE4:			return ControllerButtonCode::PADDLE4;
		case SDL_CONTROLLER_BUTTON_TOUCHPAD:		return ControllerButtonCode::TOUCHPAD;
		case SDL_CONTROLLER_BUTTON_MAX:				return ControllerButtonCode::COUNT_MAX;

		default:
			return ControllerButtonCode::INVALID;
		}
	}

	/**
	 * @brief Converts SDL controller axis code to a ControllerAxisCode.
	 * @param sdlAxisCode The SDL controller axis code.
	 * @return Corresponding ControllerAxisCode.
	 */
	inline Shared::Models::ControllerAxisCode ToControllerAxisCode(SDL_GameControllerAxis sdlAxisCode)
	{
		using namespace Shared::Models;


		switch (sdlAxisCode)
		{
		case SDL_CONTROLLER_AXIS_INVALID:			return ControllerAxisCode::INVALID;

		case SDL_CONTROLLER_AXIS_LEFTX:				return ControllerAxisCode::LEFT_STICK_X;
		case SDL_CONTROLLER_AXIS_LEFTY:				return ControllerAxisCode::LEFT_STICK_Y;
		case SDL_CONTROLLER_AXIS_RIGHTX:			return ControllerAxisCode::RIGHT_STICK_X;
		case SDL_CONTROLLER_AXIS_RIGHTY:			return ControllerAxisCode::RIGHT_STICK_Y;
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:		return ControllerAxisCode::LEFT_TRIGGER;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:		return ControllerAxisCode::RIGHT_TRIGGER;
		case SDL_CONTROLLER_AXIS_MAX:				return ControllerAxisCode::COUNT_MAX;

		default:
			return ControllerAxisCode::INVALID;
		}
	}
}