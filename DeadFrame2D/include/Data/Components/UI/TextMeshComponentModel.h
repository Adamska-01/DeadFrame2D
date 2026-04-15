#pragma once
#include "Constants/Paths/ResourcePaths.h"
#include "Core/Math/Vector2.h"
#include "Data/UI/FontStyle.h"
#include <string>
#include <string_view>


namespace DeadFrame2D::Data
{
	struct TextMeshComponentModel
	{
		std::string_view fontSource = Constants::Paths::Files::CONSOLAS_FONT;

		FontStyle fontStyle = FontStyle::NORMAL;

		std::string text = "New Text";

		SDL_Color textColor = SDL_Color{ 255, 255,  255,  255 };
	
		unsigned int fontSize = 30;

		Core::Vector2F textObjectInitialScale = Core::Vector2F::One;

		bool isCentered = false;
	};
}