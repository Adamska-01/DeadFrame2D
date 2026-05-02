#pragma once
#include <SDL.h>
#include <string>
#include <vector>


namespace DF2D::Data
{
	struct MessageBoxConfig
	{
		std::string title = "Message Box";

		std::string message = "";

		SDL_MessageBoxFlags type = SDL_MESSAGEBOX_INFORMATION;

		std::vector<SDL_MessageBoxButtonData> buttons = {};
	};
}