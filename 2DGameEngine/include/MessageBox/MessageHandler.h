#pragma once


struct MessageBoxConfig;


class MessageHandler
{
public:
	MessageHandler() = default;

	~MessageHandler() = default;


	int ShowMessageBox(const MessageBoxConfig& messageBoxConfig);
};