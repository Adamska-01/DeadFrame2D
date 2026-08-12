#pragma once
#include "Core/Services/Messaging/Abstractions/IDialogBackend.h"


struct MockDialogBackend : DF2D::Core::IDialogBackend
{
	// Configurable results
	int resultToReturn{-1};


	// Call tracking
	int showCount{0};

	DF2D::Data::MessageBoxConfig lastConfig{};


	int ShowDialogBox(const DF2D::Data::MessageBoxConfig& config) override
	{
		showCount++;
		lastConfig = config;

		return resultToReturn;
	}
};