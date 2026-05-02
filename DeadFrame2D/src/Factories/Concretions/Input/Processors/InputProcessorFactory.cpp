#include "Factories/Concretions/Input/Processors/InputProcessorFactory.h"
#include "Factories/Products/Input/Processors/ClampProcessor.h"
#include "Factories/Products/Input/Processors/DeadzoneProcessor.h"
#include "Factories/Products/Input/Processors/InvertProcessor.h"
#include "Factories/Products/Input/Processors/NormalizeProcessor.h"
#include "Factories/Products/Input/Processors/ScaleProcessor.h"
#include "Models/Input/ActionMap/Processors/Settings/ClampProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/DeadzoneProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/InvertProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/NormalizeProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/ScaleProcessorSettings.h"


namespace DF2D::Factories
{
	using namespace DF2D::Factories;
	using namespace DF2D::Models;


	std::unique_ptr<IInputProcessor> InputProcessorFactory::CreateProduct(const InputProcessor& inputProcessorModel)
	{
		switch (inputProcessorModel.kind)
		{
		case ProcessorType::DEADZONE:
		{
			if (std::holds_alternative<DeadzoneProcessorSettings>(inputProcessorModel.settings))
			{
				const auto& p = std::get<DeadzoneProcessorSettings>(inputProcessorModel.settings);
			
				return std::make_unique<DeadzoneProcessor>(p.threshold);
			}

			return std::make_unique<DeadzoneProcessor>();
		}

		case ProcessorType::SCALE:
		{
			if (std::holds_alternative<ScaleProcessorSettings>(inputProcessorModel.settings))
			{
				const auto& p = std::get<ScaleProcessorSettings>(inputProcessorModel.settings);

				return std::make_unique<ScaleProcessor>(p.factor);
			}

			return std::make_unique<ScaleProcessor>();
		}

		case ProcessorType::INVERT:
			return std::make_unique<InvertProcessor>();

		case ProcessorType::NORMALIZE:
			return std::make_unique<NormalizeProcessor>();

		case ProcessorType::CLAMP:
		{
			if (std::holds_alternative<ClampProcessorSettings>(inputProcessorModel.settings))
			{
				const auto& p = std::get<ClampProcessorSettings>(inputProcessorModel.settings);

				return std::make_unique<ClampProcessor>(p.min, p.max);
			}

			return std::make_unique<ClampProcessor>();
		}
		}
	}
}