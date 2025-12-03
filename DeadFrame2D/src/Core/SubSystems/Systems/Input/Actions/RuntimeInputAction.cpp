#include "Core/SubSystems/Systems/Input/Actions/RuntimeInputAction.h"


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;


	RuntimeInputAction::RuntimeInputAction(const std::string& name, ValueType valueType, std::vector<Shared::Models::Binding> bindings)
		: name(name),
		bindings(bindings),
		phase(ActionPhase::WAITING)
	{
		switch (valueType)
		{
			case ValueType::BOOL:
				value = previousValue = false;
				break;
			case ValueType::FLOAT:
				value = previousValue = 0.0f;
				break;
			case ValueType::VECTOR2:
				value = previousValue = Vector2F::Zero;
				break;
			default:
				break;
		}
	}

	void RuntimeInputAction::ResetFrame()
	{
		previousValue = value;

		if (std::holds_alternative<bool>(value))
		{
			value = false;
		}
		else if (std::holds_alternative<float>(value))
		{
			value = 0.0f;
		}
		else if (std::holds_alternative<Vector2F>(value))
		{
			value = Vector2F::Zero;
		}

		phase = ActionPhase::WAITING;
	}

	const std::vector<Binding>& RuntimeInputAction::GetBindings() const
	{
		return bindings;
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