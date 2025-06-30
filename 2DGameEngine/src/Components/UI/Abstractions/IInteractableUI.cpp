#include "Components/UI/Abstractions/IInteractableUI.h"


bool IInteractableUI::IsPressed() const
{
	return isPressed;
}

bool IInteractableUI::IsHovered() const
{
	return isHovered;
}