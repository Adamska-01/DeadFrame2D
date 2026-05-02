#pragma once
#include "Data/Services/Messaging/MessageBoxConfig.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API SystemDialogBox
	{
	private:
		SystemDialogBox() = delete;

		SystemDialogBox(const SystemDialogBox&) = delete;


		SystemDialogBox& operator=(const SystemDialogBox&) = delete;


		static int ShowBasicBox(const std::string& title, const std::string& message, SDL_MessageBoxFlags type, const std::vector<SDL_MessageBoxButtonData>& buttons);


	public:
		static int ShowDialogBox(const Data::MessageBoxConfig& config);


		// Helpers
		static void ShowInfoBox(const std::string& title, const std::string& message);

		static void ShowErrorBox(const std::string& title, const std::string& message);

		static bool ShowConfirmBox(const std::string& title, const std::string& question);
	};
}