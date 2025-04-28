#include "Components/UI/Abstractions/INavigableUI.h"


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