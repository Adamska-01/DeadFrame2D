#pragma once
#include "DF2D_API.h"


namespace DeadFrame2D::Data
{
	struct MessageBoxConfig;
}


namespace DeadFrame2D::Core
{
	// TODO: Rename this to "MessageBoxService"
	class DF2D_API MessageHandler
	{
	public:
		MessageHandler() = default;

		~MessageHandler() = default;


		int ShowMessageBox(const DeadFrame2D::Data::MessageBoxConfig& messageBoxConfig);
	};
}