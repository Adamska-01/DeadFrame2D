#include "Core/SubSystems/Systems/Input/Actions/RuntimeInputAction.h"


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;


	RuntimeInputAction::RuntimeInputAction(const std::string& name, ValueType valueType, std::vector<Shared::Models::Binding> bindings)
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
	}

	RuntimeInputAction::RuntimeInputAction(const RuntimeInputAction& other)
		: name(other.name),
		value(other.value),
		previousValue(other.previousValue),
		pendingValue(other.pendingValue),
		isValuePending(other.isValuePending),
		phase(other.phase),
		bindings(other.bindings)
	{
		// listeners are intentionally NOT copied
	}

	RuntimeInputAction::RuntimeInputAction(RuntimeInputAction&& other) noexcept
		: name(std::move(other.name)), 
		value(std::move(other.value)),
		previousValue(std::move(other.previousValue)),
		pendingValue(std::move(other.pendingValue)),
		isValuePending(other.isValuePending),
		phase(other.phase),
		bindings(std::move(other.bindings))
	{
		// listeners are intentionally NOT moved
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
		// listeners are intentionally NOT copied

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
		// listeners are intentionally NOT moved

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