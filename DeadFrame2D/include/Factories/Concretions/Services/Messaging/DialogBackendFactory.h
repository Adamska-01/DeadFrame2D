#pragma once
#include "Core/Services/Messaging/Abstractions/IDialogBackend.h"
#include <memory>


namespace DF2D::Factories
{
	class DialogBackendFactory
	{
	public:
		std::unique_ptr<Core::IDialogBackend> CreateProduct();
	};
}