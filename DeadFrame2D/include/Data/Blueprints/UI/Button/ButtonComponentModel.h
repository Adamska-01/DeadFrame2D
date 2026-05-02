#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Button/ButtonCallback.h"
#include <optional>
#include <string_view>


namespace DF2D::Data
{
	struct ButtonComponentModel
	{
		std::optional<ButtonCallback> onPressedHandler;

		std::optional<ButtonCallback> onEnterHandler;

		std::string_view idleButtonSource = "";

		std::string_view hoveredButtonSource = "";

		std::string_view pressedButtonSource = "";

		Core::Vector2F buttonSize = Core::Vector2F(100.f, 50.f);
	};
}