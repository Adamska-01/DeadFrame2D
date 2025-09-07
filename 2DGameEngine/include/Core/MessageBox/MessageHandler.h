#pragma once


struct MessageBoxConfig;


// TODO: Rename this to "MessageBoxService"
class MessageHandler
{
public:
	MessageHandler() = default;

	~MessageHandler() = default;


	int ShowMessageBox(const MessageBoxConfig& messageBoxConfig);
};