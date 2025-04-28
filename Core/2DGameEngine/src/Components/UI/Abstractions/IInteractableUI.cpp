#include "Components/UI/Abstractions/IInteractableUI.h"


bool IInteractableUI::IsPressed() const
{
	return isPressed;
}

bool IInteractableUI::IsHovered() const
{
	return isHovered;
}

void IInteractableUI::NavigateUp()
{
	if (up == nullptr)
		return;

	OnPointerExit();

	up->OnPointerEnter();
}

void IInteractableUI::NavigateDown()
{
	if (down == nullptr)
		return;

	OnPointerExit();

	up->NavigateDown();
}

void IInteractableUI::NavigateRight()
{
	if (right == nullptr)
		return;

	OnPointerExit();

	up->NavigateRight();
}

void IInteractableUI::NavigateLeft()
{
	if (left == nullptr)
		return;

	OnPointerExit();

	up->NavigateLeft();
}