#pragma once
#include "Data/Services/Messaging/MessageBoxConfig.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	class DF2D_API IDialogBackend
	{
	public:
		virtual ~IDialogBackend() = default;


		/**
		 * @brief Shows a blocking, platform-native dialog box.
		 * @return The id of the picked button, or -1 when the dialog was dismissed or failed to open.
		 */
		virtual int ShowDialogBox(const Data::MessageBoxConfig& config) = 0;
	};
}