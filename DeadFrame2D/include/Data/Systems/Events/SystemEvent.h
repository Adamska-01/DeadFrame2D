#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "Models/Input/Controls/ControllerAxisCode.h"
#include "Models/Input/Controls/ControllerButtonCode.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"
#include "Models/Input/Controls/MouseButtonCode.h"
#include <string>
#include <variant>


namespace DF2D::Data
{
	struct QuitEvent
	{
	};


	struct KeyEvent
	{
		Models::KeyboardKeyCode key = Models::KeyboardKeyCode::UNKNOWN;

		bool pressed = false;
	};


	struct MouseButtonEvent
	{
		Models::MouseButtonCode button = Models::MouseButtonCode::UNKNOWN;

		bool pressed = false;

		Core::Vector2F position;
	};


	struct MouseMoveEvent
	{
		Core::Vector2F position;

		Core::Vector2F delta;
	};


	struct MouseWheelEvent
	{
		Core::Vector2F delta;
	};


	struct ControllerButtonEvent
	{
		InputDeviceID deviceID = 0;

		Models::ControllerButtonCode button;

		bool pressed = false;
	};


	struct ControllerAxisEvent
	{
		InputDeviceID deviceID = 0;

		Models::ControllerAxisCode axis;

		/** @brief Normalized to [-1, 1]. */
		float value = 0.0f;
	};


	struct ControllerConnectedEvent
	{
		InputDeviceID deviceID = 0;

		std::string name;
	};


	struct ControllerDisconnectedEvent
	{
		InputDeviceID deviceID = 0;
	};


	using SystemEvent = std::variant<
		QuitEvent,
		KeyEvent,
		MouseButtonEvent,
		MouseMoveEvent,
		MouseWheelEvent,
		ControllerButtonEvent,
		ControllerAxisEvent,
		ControllerConnectedEvent,
		ControllerDisconnectedEvent>;
}
