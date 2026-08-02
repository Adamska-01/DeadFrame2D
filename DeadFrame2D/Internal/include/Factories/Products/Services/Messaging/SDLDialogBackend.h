#pragma once
#include "Core/Services/Messaging/Abstractions/IDialogBackend.h"
#include "DF2D_API.h"


namespace DF2D::Internal
{
	class DF2D_API SDLDialogBackend : public Core::IDialogBackend
	{
	public:
		SDLDialogBackend() = default;

		~SDLDialogBackend() override = default;


		int ShowDialogBox(const Data::MessageBoxConfig& config) override;
	};
}