#pragma once
#include "DF2D_API.h"
#include <SDL.h>
#include <string>
#include <vector>


namespace DeadFrame2D::Data
{
	struct DF2D_API MessageBoxConfig
	{
		std::string title = "Default Title";
	
		std::string message = "Default Message";
	
		SDL_MessageBoxFlags type = SDL_MESSAGEBOX_INFORMATION;
	
		std::vector<SDL_MessageBoxButtonData> buttons;


		MessageBoxConfig(
			const std::string& title, 
			const std::string& message, 
			SDL_MessageBoxFlags boxType, 
			const std::vector<SDL_MessageBoxButtonData>& buttons);


		void AddButton(const SDL_MessageBoxButtonData& button);

		bool IsValid() const;
	};
}