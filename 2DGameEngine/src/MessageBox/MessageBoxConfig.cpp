#include <iostream>
#include <MessageBox/MessageBoxConfig.h>


MessageBoxConfig::MessageBoxConfig(
	const std::string& t,
	const std::string& m,
	SDL_MessageBoxFlags ty,
	const std::vector<SDL_MessageBoxButtonData>& b) 
	: title(t), message(m), type(ty), buttons(b) 
{
}

void MessageBoxConfig::AddButton(const SDL_MessageBoxButtonData& button) 
{
	buttons.push_back(button);
}

bool MessageBoxConfig::IsValid() const 
{
	return !title.empty() && !message.empty() && !buttons.empty();
}