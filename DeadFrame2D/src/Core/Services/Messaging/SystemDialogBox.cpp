#include "Core/Services/Messaging/SystemDialogBox.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;


	int SystemDialogBox::ShowBasicBox(const std::string& title, const std::string& message, SDL_MessageBoxFlags type, const std::vector<SDL_MessageBoxButtonData>& buttons)
	{
		auto config = MessageBoxConfig
		{
			.title = title,
			.message = message,
			.type = type,
			.buttons = buttons
		};

		return ShowDialogBox(config);
	}


	int SystemDialogBox::ShowDialogBox(const MessageBoxConfig& config)
	{
		auto& buttons = config.buttons;

		auto messageBoxData = SDL_MessageBoxData
		{
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


	void SystemDialogBox::ShowInfoBox(const std::string& title, const std::string& message)
	{
		ShowBasicBox(title, message, SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, {});
	}

	void SystemDialogBox::ShowErrorBox(const std::string& title, const std::string& message)
	{
		ShowBasicBox(title, message, SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, {});
	}

	bool SystemDialogBox::ShowConfirmBox(const std::string& title, const std::string& question)
	{
		auto basicBoxResult = ShowBasicBox(
			title,
			question,
			SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING,
			{
				{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yes" },
				{ 0, 1, "No" }
			});

		return basicBoxResult == 0;
	}
}