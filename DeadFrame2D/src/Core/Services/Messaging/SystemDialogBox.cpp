#include "Core/Services/Messaging/SystemDialogBox.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	SystemDialogBox::SystemDialogBox(std::unique_ptr<IDialogBackend> dialogBackend)
		: dialogBackend(std::move(dialogBackend))
	{
		Guard::AgainstNull(this->dialogBackend.get(), NAME_OF(dialogBackend));
	}


	int SystemDialogBox::ShowBasicBox(const std::string& title, const std::string& message, MessageBoxType type, std::vector<MessageBoxButton> buttons)
	{
		auto config = MessageBoxConfig
		{
			.title = title,
			.message = message,
			.type = type,
			.buttons = std::move(buttons)
		};

		return ShowDialogBox(config);
	}


	int SystemDialogBox::ShowDialogBox(const MessageBoxConfig& config)
	{
		return dialogBackend->ShowDialogBox(config);
	}


	void SystemDialogBox::ShowInfoBox(const std::string& title, const std::string& message)
	{
		ShowBasicBox(title, message, MessageBoxType::INFORMATION, {});
	}

	void SystemDialogBox::ShowErrorBox(const std::string& title, const std::string& message)
	{
		ShowBasicBox(title, message, MessageBoxType::CRITICAL, {});
	}

	bool SystemDialogBox::ShowConfirmBox(const std::string& title, const std::string& question)
	{
		auto basicBoxResult = ShowBasicBox(
			title,
			question,
			MessageBoxType::WARNING,
			{
				{ CONFIRM_YES_ID, "Yes", MessageBoxButtonFlags::RETURN_KEY_DEFAULT },
				{ CONFIRM_NO_ID, "No", MessageBoxButtonFlags::ESCAPE_KEY_DEFAULT }
			});

		return basicBoxResult == CONFIRM_YES_ID;
	}
}