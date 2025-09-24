#pragma once
#include "Engine/Components/UI/Abstractions/IInteractableUI.h"
#include "Engine/EngineEvents//DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class NewInteractableSelectedEvent : public DispatchableEvent
	{
	public:
		IInteractableUI* newSelectedInteractable;


		NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable);
	};
}