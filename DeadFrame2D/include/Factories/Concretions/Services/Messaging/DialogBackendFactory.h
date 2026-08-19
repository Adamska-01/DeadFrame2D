#pragma once
#include "Core/Services/Messaging/Abstractions/IDialogBackend.h"
#include "DF2D_API.h"
#include "Factories/Abstractions/IFactory.h"
#include <memory>


namespace DF2D::Factories
{
	class DF2D_API DialogBackendFactory : public IFactory<std::unique_ptr<Core::IDialogBackend>>
	{
	public:
		std::unique_ptr<Core::IDialogBackend> CreateProduct() override;
	};
}