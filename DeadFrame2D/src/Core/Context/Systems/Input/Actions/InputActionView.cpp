#include "Core/Context/Systems/Input/Actions/InputActionView.h"


namespace DF2D::Core
{
	InputActionView::InputActionView(ActionPhase phase, std::variant<bool, float, Vector2F> value)
		: phase(phase),
		value(value)
	{
	}


	bool InputActionView::IsWaiting() const
	{
		return phase == ActionPhase::WAITING;
	}

	bool InputActionView::IsStarted() const
	{
		return phase == ActionPhase::STARTED;
	}

	bool InputActionView::IsPerformed() const
	{
		return phase == ActionPhase::PERFORMED;
	}

	bool InputActionView::IsCancelled() const
	{
		return phase == ActionPhase::CANCELED;
	}
}