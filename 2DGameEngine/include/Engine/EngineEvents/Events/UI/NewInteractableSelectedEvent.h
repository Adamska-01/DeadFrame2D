#pragma once
#include "DF2D_API.h"
#include "Engine/Components/UI/Abstractions/IInteractableUI.h"
#include "Engine/EngineEvents//DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API NewInteractableSelectedEvent : public DispatchableEvent
	{
	public:
		IInteractableUI* newSelectedInteractable;


		NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable);
	};
}