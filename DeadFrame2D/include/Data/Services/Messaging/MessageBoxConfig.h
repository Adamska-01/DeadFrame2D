#pragma once
#include "Data/Services/Messaging/MessageBoxButton.h"
#include "Data/Services/Messaging/MessageBoxType.h"
#include <string>
#include <vector>


namespace DF2D::Data
{
	struct MessageBoxConfig
	{
		std::string title = "Message Box";

		std::string message = "";

		MessageBoxType type = MessageBoxType::INFORMATION;

		/** @brief Empty means a single platform-provided dismiss button. */
		std::vector<MessageBoxButton> buttons = {};
	};
}