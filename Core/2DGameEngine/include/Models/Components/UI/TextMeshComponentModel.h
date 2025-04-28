#pragma once
#include "Data/UI/FontStyle.h"
#include <Constants/SharedResourcePaths.h>
#include <string>
#include <string_view>


struct TextMeshComponentModel
{
	std::string_view fontSource = SharedResourcePaths::Fonts::CONSOLAS_FONT_PATH;

	FontStyle fontStyle = FontStyle::NORMAL;

	std::string text = "New Text";

	SDL_Color textColor = SDL_Color{ 255, 255,  255,  255 };
	
	unsigned int fontSize = 30;

	Vector2F textObjectInitialScale = Vector2F::One;
	
	unsigned int linesNumber = 1;
};