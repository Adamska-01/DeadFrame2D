#pragma once
#include "Components/UI/Abstractions/IInteractableUI.h"
#include "EventSystem/DispatchableEvent.h"


class NewInteractableSelectedEvent : public DispatchableEvent
{
public:
	IInteractableUI* newSelectedInteractable;


	NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable);
};