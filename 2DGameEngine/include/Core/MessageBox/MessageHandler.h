#pragma once


namespace DeadFrame2D::Data
{
	struct MessageBoxConfig;
}


namespace DeadFrame2D::Core
{
	// TODO: Rename this to "MessageBoxService"
	class MessageHandler
	{
	public:
		MessageHandler() = default;

		~MessageHandler() = default;


		int ShowMessageBox(const DeadFrame2D::Data::MessageBoxConfig& messageBoxConfig);
	};
}