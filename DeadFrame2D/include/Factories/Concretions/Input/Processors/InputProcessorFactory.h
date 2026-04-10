#pragma once
#include "Factories/Abstractions/IFactoryWithInitialiser.h"
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"
#include "Models/Input/ActionMap/Processors/InputProcessor.h"
#include <memory>


namespace DeadFrame2D::Factories
{
	class InputProcessorFactory : public IFactoryWithInitialiser<std::unique_ptr<DeadFrame2D::Factories::IInputProcessor>, const Shared::Models::InputProcessor&>
	{
	public:
		std::unique_ptr<DeadFrame2D::Factories::IInputProcessor> CreateProduct(const Shared::Models::InputProcessor& inputProcessorModel) override;
	};
}