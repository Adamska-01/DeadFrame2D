#pragma once
#include "Core/Services/Messaging/Abstractions/IDialogBackend.h"
#include "Data/Services/Messaging/MessageBoxConfig.h"
#include "DF2D_API.h"
#include <memory>
#include <string>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API SystemDialogBox
	{
	private:
		std::unique_ptr<IDialogBackend> dialogBackend;


		int ShowBasicBox(const std::string& title, const std::string& message, Data::MessageBoxType type, std::vector<Data::MessageBoxButton> buttons);


	public:
		/** @brief Returned when no button was picked (dialog dismissed or failed to open). */
		static constexpr int NO_BUTTON_SELECTED = -1;

		/** @brief Button ids used by ShowConfirmBox. */
		static constexpr int CONFIRM_YES_ID = 0;

		static constexpr int CONFIRM_NO_ID = 1;


		SystemDialogBox(std::unique_ptr<IDialogBackend> dialogBackend);

		~SystemDialogBox() = default;

		SystemDialogBox(const SystemDialogBox&) = delete;

		SystemDialogBox(SystemDialogBox&&) = delete;

		SystemDialogBox& operator=(const SystemDialogBox&) = delete;

		SystemDialogBox& operator=(SystemDialogBox&&) = delete;


		int ShowDialogBox(const Data::MessageBoxConfig& config);


		// Helpers
		void ShowInfoBox(const std::string& title, const std::string& message);

		void ShowErrorBox(const std::string& title, const std::string& message);

		bool ShowConfirmBox(const std::string& title, const std::string& question);
	};
}