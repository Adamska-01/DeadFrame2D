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