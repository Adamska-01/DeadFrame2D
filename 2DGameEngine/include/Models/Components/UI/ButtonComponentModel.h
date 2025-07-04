#pragma once
#include "Math/Vector2.h"
#include <functional>
#include <string_view>


struct ButtonComponentModel
{
	std::uintptr_t identifier = 0;

	std::function<void()> onPressedHandler = []() {};

	std::function<void()> onEnterHandler = []() {};

	std::string_view idleButtonSource = "";
	
	std::string_view hoveredButtonSource = "";
	
	std::string_view pressedButtonSource = "";
	
	Vector2F buttonSize = Vector2F{ 100.f, 50.f };
};