#include "Core/Context/Systems/Input/Actions/RuntimeInputAction.h"
#include "Factories/Concretions/Input/Processors/InputProcessorFactory.h"


namespace DF2D::Core
{
	using namespace DF2D::Factories;
	using namespace DF2D::Models;


	RuntimeInputAction::RuntimeInputAction(
		const std::string& name,
		ValueType valueType,
		std::vector<Binding> bindings,
		std::vector<InputProcessor> inputProcessors)
		: name(name),
		bindings(bindings),
		isValuePending(false),
		phase(ActionPhase::WAITING)
	{
		switch (valueType)
		{
			case ValueType::BOOL:
				value = previousValue = pendingValue = false;
				break;

			case ValueType::FLOAT:
				value = previousValue = pendingValue = 0.0f;
				break;

			case ValueType::VECTOR2:
				value = previousValue = pendingValue = Vector2F::Zero;
				break;

			default:
				break;
		}

		auto inputProcessorFactory = InputProcessorFactory();

		for (const auto& processor : inputProcessors)
		{
			processors.push_back(inputProcessorFactory.CreateProduct(processor));
		}
	}


	void RuntimeInputAction::ResetFrame()
	{
		previousValue = value;

		auto isHeld = phase == ActionPhase::PERFORMED;

		if (std::holds_alternative<bool>(value))
		{
			value = isHeld ? value : false;
			pendingValue = false;
		}
		else if (std::holds_alternative<float>(value))
		{
			value = isHeld ? value : 0.0f;
			pendingValue = 0.0f;
		}
		else if (std::holds_alternative<Vector2F>(value))
		{
			value = isHeld ? value : Vector2F::Zero;
			pendingValue = Vector2F::Zero;
		}

		phase = isHeld ? phase : ActionPhase::WAITING;

		isValuePending = false;
	}

	void RuntimeInputAction::ApplyProcessors()
	{
		if (processors.empty())
			return;

		if (std::holds_alternative<bool>(value))
			return;

		std::visit(
			[this](auto& v)
			{
				using T = std::decay_t<decltype(v)>;

				if constexpr (std::is_same_v<T, float> || std::is_same_v<T, Vector2F>)
				{
					for (const auto& processor : processors)
					{
						processor->Process(v);
					}
				}
			},
			value);
	}
}