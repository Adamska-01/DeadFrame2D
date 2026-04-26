#include "Engine/Events/UI/NewInteractableSelectedEvent.h"


namespace DeadFrame2D::Engine
{
	NewInteractableSelectedEvent::NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable)
		: newSelectedInteractable(newSelectedInteractable)
	{
	}
}