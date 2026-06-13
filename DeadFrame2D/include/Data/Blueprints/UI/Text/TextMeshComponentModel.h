#pragma once
#include "Constants/Paths/ResourcePaths.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include <string>
#include <string_view>


namespace DF2D::Data
{
	struct TextMeshComponentModel
	{
		std::string_view fontSource = Constants::Paths::Files::CONSOLAS_FONT;

		FontStyle fontStyle = FontStyle::NORMAL;

		std::string text = "New Text";

		Core::Color textColor = Core::Color{ 255, 255, 255, 255 };

		unsigned int fontSize = 30;

		Core::Vector2F textObjectInitialScale = Core::Vector2F::One;

		bool isCentered = false;
	};
}