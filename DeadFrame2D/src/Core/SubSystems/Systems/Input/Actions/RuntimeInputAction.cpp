#include "Core/SubSystems/Systems/Input/Actions/RuntimeInputAction.h"
#include "Factories/Concretions/Input/Processors/InputProcessorFactory.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Factories;

	using namespace Shared::Models;


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

	RuntimeInputAction::RuntimeInputAction(const RuntimeInputAction& other)
		: name(other.name),
		value(other.value),
		previousValue(other.previousValue),
		pendingValue(other.pendingValue),
		isValuePending(other.isValuePending),
		phase(other.phase),
		bindings(other.bindings),
		processors()
	{
		for (const auto& processor : other.processors)
		{
			processors.push_back(processor->Clone());
		}
	}

	RuntimeInputAction::RuntimeInputAction(RuntimeInputAction&& other) noexcept
		: name(std::move(other.name)), 
		value(std::move(other.value)),
		previousValue(std::move(other.previousValue)),
		pendingValue(std::move(other.pendingValue)),
		isValuePending(other.isValuePending),
		phase(other.phase),
		bindings(std::move(other.bindings)),
		processors(std::move(other.processors))
	{
	}

	RuntimeInputAction& RuntimeInputAction::operator=(const RuntimeInputAction& other)
	{
		if (this == &other)
			return *this;

		name = other.name;
		value = other.value;
		previousValue = other.previousValue;
		pendingValue = other.pendingValue;
		isValuePending = other.isValuePending;
		phase = other.phase;
		bindings = other.bindings;
		processors.clear();

		for (const auto& processor : other.processors)
		{
			processors.push_back(processor->Clone());
		}

		return *this;
	}

	RuntimeInputAction& RuntimeInputAction::operator=(RuntimeInputAction&& other) noexcept
	{
		if (this == &other)
			return *this;

		name = std::move(other.name);
		value = std::move(other.value);
		previousValue = std::move(other.previousValue);
		pendingValue = std::move(other.pendingValue);
		isValuePending = other.isValuePending;
		phase = other.phase;
		bindings = std::move(other.bindings);
		processors = std::move(other.processors);

		return *this;
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

	bool RuntimeInputAction::IsWaiting() const
	{
		return phase == ActionPhase::WAITING;
	}

	bool RuntimeInputAction::IsStarted() const
	{
		return phase == ActionPhase::STARTED;
	}

	bool RuntimeInputAction::IsPerformed() const
	{
		return phase == ActionPhase::PERFORMED;
	}

	bool RuntimeInputAction::IsCancelled() const
	{
		return phase == ActionPhase::CANCELED;
	}
}