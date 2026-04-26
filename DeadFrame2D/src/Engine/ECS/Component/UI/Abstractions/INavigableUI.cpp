#include "Engine/ECS/Component/UI/Abstractions/IInteractableUI.h"
#include "Engine/ECS/Component/UI/Abstractions/INavigableUI.h"


namespace DeadFrame2D::Engine
{
	ComponentHandle<IInteractableUI> INavigableUI::GetUpInteractable()
	{
		return up;
	}

	ComponentHandle<IInteractableUI> INavigableUI::GetDownInteractable()
	{
		return down;
	}

	ComponentHandle<IInteractableUI> INavigableUI::GetRightInteractable()
	{
		return right;
	}

	ComponentHandle<IInteractableUI> INavigableUI::GetLeftInteractable()
	{
		return left;
	}

	void INavigableUI::SetNavigableUpElement(ComponentHandle<IInteractableUI> upElement)
	{
		up = upElement;
	}

	void INavigableUI::SetNavigableDownElement(ComponentHandle<IInteractableUI> downElement)
	{
		down = downElement;
	}

	void INavigableUI::SetNavigableRightElement(ComponentHandle<IInteractableUI> rightElement)
	{
		right = rightElement;
	}

	void INavigableUI::SetNavigableLeftElement(ComponentHandle<IInteractableUI> leftElement)
	{
		left = leftElement;
	}
}