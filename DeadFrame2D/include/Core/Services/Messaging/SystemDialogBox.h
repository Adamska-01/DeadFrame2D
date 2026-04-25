#pragma once
#include "Data/Window/MessageBoxConfig.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API SystemDialogBox
	{
	private:
		SystemDialogBox() = delete;

		SystemDialogBox(const SystemDialogBox&) = delete;


		SystemDialogBox& operator=(const SystemDialogBox&) = delete;


		static int ShowBasicBox(const std::string& title, const std::string& message, SDL_MessageBoxFlags type, const std::vector<SDL_MessageBoxButtonData>& buttons);


	public:
		static int ShowDialogBox(const DeadFrame2D::Data::MessageBoxConfig& config);


		// Helpers
		static void ShowInfoBox(const std::string& title, const std::string& message);

		static void ShowErrorBox(const std::string& title, const std::string& message);

		static bool ShowConfirmBox(const std::string& title, const std::string& question);
	};
}