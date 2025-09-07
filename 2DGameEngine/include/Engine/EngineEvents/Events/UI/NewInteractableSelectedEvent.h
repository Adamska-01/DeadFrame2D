#pragma once
#include "Engine/Components/UI/Abstractions/IInteractableUI.h"
#include "Engine/EngineEvents//DispatchableEvent.h"


class NewInteractableSelectedEvent : public DispatchableEvent
{
public:
	IInteractableUI* newSelectedInteractable;


	NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable);
};