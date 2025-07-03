#pragma once
#include "Data/UI/FontStyle.h"
#include <Constants/ResourcePaths.h>
#include <string>
#include <string_view>


struct TextMeshComponentModel
{
	std::string_view fontSource = Constants::ResourcePaths::Files::CONSOLAS_FONT;

	FontStyle fontStyle = FontStyle::NORMAL;

	std::string text = "New Text";

	SDL_Color textColor = SDL_Color{ 255, 255,  255,  255 };
	
	unsigned int fontSize = 30;

	Vector2F textObjectInitialScale = Vector2F::One;

	bool isCentered = false;
};