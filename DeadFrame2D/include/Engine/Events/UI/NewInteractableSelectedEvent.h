#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Abstractions/IInteractableUI.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API NewInteractableSelectedEvent : public DispatchableEvent
	{
	public:
		IInteractableUI* newSelectedInteractable;


		NewInteractableSelectedEvent(IInteractableUI* newSelectedInteractable);
	};
}