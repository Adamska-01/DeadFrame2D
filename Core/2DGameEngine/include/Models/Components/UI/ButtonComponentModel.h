#pragma once
#include "Math/Vector2.h"
#include <functional>
#include <string_view>


struct ButtonComponentModel
{
	std::function<void()> onPressedHandler = []() {};

	std::string_view idleButtonSource = "";
	
	std::string_view hoveredButtonSource = "";
	
	std::string_view pressedButtonSource = "";
	
	Vector2F buttonSize = Vector2F{ 100.f, 50.f };
};