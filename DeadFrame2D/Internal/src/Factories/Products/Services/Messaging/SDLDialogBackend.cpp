#include "Converters/Messaging/MessageBoxConversions.h"
#include "Factories/Products/Services/Messaging/SDLDialogBackend.h"
#include <vector>


namespace DF2D::Internal
{
	using namespace DF2D::Data;


	constexpr int NO_BUTTON_SELECTED = -1;


	int SDLDialogBackend::ShowDialogBox(const MessageBoxConfig& config)
	{
		auto sdlButtons = std::vector<SDL_MessageBoxButtonData>();
		sdlButtons.reserve(config.buttons.size());

		for (const auto& button : config.buttons)
		{
			sdlButtons.push_back(SDL_MessageBoxButtonData
				{
					MessageBoxConversions::ToSDLButtonFlags(button.flags),
					button.id,
					button.text.c_str()
				});
		}

		auto messageBoxData = SDL_MessageBoxData
		{
			static_cast<Uint32>(MessageBoxConversions::ToSDLMessageBoxFlags(config.type)),
			nullptr,
			config.title.c_str(),
			config.message.c_str(),
			static_cast<int>(sdlButtons.size()),
			sdlButtons.empty() ? nullptr : sdlButtons.data(),
			nullptr
		};

		auto buttonId = NO_BUTTON_SELECTED;

		if (SDL_ShowMessageBox(&messageBoxData, &buttonId) < 0)
		{
			SDL_Log("Error displaying message box: %s", SDL_GetError());

			return NO_BUTTON_SELECTED;
		}

		return buttonId;
	}
}