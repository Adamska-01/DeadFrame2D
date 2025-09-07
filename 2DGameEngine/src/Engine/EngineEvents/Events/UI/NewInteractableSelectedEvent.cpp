#include "Engine/EngineEvents/Events/UI/NewInteractableSelectedEvent.h"


NewInteractableSelectedEvent::NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable)
	: newSelectedInteractable(newSelectedInteractable)
{
}