#pragma once
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"
#include "Factories/Abstractions/IFactoryWithInitialiser.h"
#include "Models/Window/WindowConfig.h"
#include <memory>


namespace DF2D::Factories
{
	class WindowBackendFactory : public IFactoryWithInitialiser<std::unique_ptr<Core::IWindowBackend>, Models::WindowConfig>
	{
	public:
		std::unique_ptr<Core::IWindowBackend> CreateProduct(Models::WindowConfig config) override;
	};
}