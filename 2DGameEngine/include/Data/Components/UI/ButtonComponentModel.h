#pragma once
#include "Core/Math/Vector2.h"
#include <functional>
#include <string_view>


namespace DeadFrame2D::Data
{
	struct ButtonComponentModel
	{
		std::uintptr_t identifier = 0;

		std::function<void()> onPressedHandler = []() {};

		std::function<void()> onEnterHandler = []() {};

		std::string_view idleButtonSource = "";
	
		std::string_view hoveredButtonSource = "";
	
		std::string_view pressedButtonSource = "";
	
		DeadFrame2D::Core::Vector2F buttonSize = DeadFrame2D::Core::Vector2F(100.f, 50.f);
	};
}