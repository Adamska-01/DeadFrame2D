#pragma once
#include "Data/Systems/Events/SystemEvents.h"
#include <variant>


namespace DF2D::Data
{
	using SystemEvent = std::variant<
		QuitEvent,
		KeyEvent,
		MouseButtonEvent,
		MouseMoveEvent,
		MouseWheelEvent,
		TextInputEvent,
		ControllerButtonEvent,
		ControllerAxisEvent,
		ControllerConnectedEvent,
		ControllerDisconnectedEvent>;
}