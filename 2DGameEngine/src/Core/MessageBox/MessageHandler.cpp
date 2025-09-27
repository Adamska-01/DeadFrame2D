#include "Core/MessageBox/MessageHandler.h"
#include "Data/Window/MessageBoxConfig.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;


	int MessageHandler::ShowMessageBox(const MessageBoxConfig& config)
	{
		auto& buttons = config.buttons;

		SDL_MessageBoxData messageBoxData = {
			static_cast<Uint32>(config.type),
			nullptr,
			config.title.c_str(),
			config.message.c_str(),
			static_cast<int>(buttons.size()),
			buttons.empty() ? nullptr : buttons.data(),
			nullptr 
		};

		auto buttonId = -1;
	
		if (SDL_ShowMessageBox(&messageBoxData, &buttonId) < 0) 
		{
			SDL_Log("Error displaying message box: %s", SDL_GetError());
		}

		return buttonId;
	}
}