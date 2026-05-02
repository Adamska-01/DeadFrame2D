#pragma once
#include "Factories/Abstractions/IFactoryWithInitialiser.h"
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"
#include "Models/Input/ActionMap/Processors/InputProcessor.h"
#include <memory>


namespace DF2D::Factories
{
	class InputProcessorFactory : public IFactoryWithInitialiser<std::unique_ptr<IInputProcessor>, const Models::InputProcessor&>
	{
	public:
		std::unique_ptr<IInputProcessor> CreateProduct(const Models::InputProcessor& inputProcessorModel) override;
	};
}