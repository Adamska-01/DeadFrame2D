#include "EventSystem/Events/UI/NewInteractableSelectedEvent.h"


NewInteractableSelectedEvent::NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable)
	: newSelectedInteractable(newSelectedInteractable)
{
}