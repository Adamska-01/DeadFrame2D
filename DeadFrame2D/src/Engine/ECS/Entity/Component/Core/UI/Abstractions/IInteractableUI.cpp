#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"


namespace DF2D::Engine
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