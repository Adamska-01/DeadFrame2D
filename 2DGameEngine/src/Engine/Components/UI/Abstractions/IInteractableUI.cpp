#include "Engine/Components/UI/Abstractions/IInteractableUI.h"


namespace DeadFrame2D::Engine
{
	bool IInteractableUI::IsPressed() const
	{
		return isPressed;
	}

	bool IInteractableUI::IsHovered() const
	{
		return isHovered;
	}
}