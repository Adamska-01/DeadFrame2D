#include "Components/UI/Abstractions/INavigableUI.h"


IInteractableUI* INavigableUI::GetUpInteractable()
{
	return up;
}

IInteractableUI* INavigableUI::GetDownInteractable()
{
	return down;
}

IInteractableUI* INavigableUI::GetRightInteractable()
{
	return right;
}

IInteractableUI* INavigableUI::GetLeftInteractable()
{
	return left;
}

void INavigableUI::SetNavigableUpElement(IInteractableUI* upElement)
{
	up = upElement;
}

void INavigableUI::SetNavigableDownElement(IInteractableUI* downElement)
{
	down = downElement;
}

void INavigableUI::SetNavigableRightElement(IInteractableUI* rightElement)
{
	right = rightElement;
}

void INavigableUI::SetNavigableLeftElement(IInteractableUI* leftElement)
{
	left = leftElement;
}