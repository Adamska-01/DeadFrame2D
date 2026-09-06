#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	void IInteractableUI::HandleUIEvent(UIEventType eventType, const UIEventPayload& payload)
	{
		// One rule, applied in one place: a widget that is not interactable takes part in no interaction
		// at all. Not being hit-tested already stops the pointer reaching it, but events raised by any
		// other route -- focus, or keyboard activation -- would otherwise still get through.
		if (!interactable)
			return;

		switch (eventType)
		{
		case UIEventType::CLICK:
			OnClick.Broadcast();
			break;

		case UIEventType::POINTER_ENTER:
			OnPointerEnter.Broadcast();
			break;

		case UIEventType::POINTER_EXIT:
			OnPointerExit.Broadcast();
			break;

		case UIEventType::POINTER_DOWN:
			OnPointerDown.Broadcast();
			break;

		case UIEventType::POINTER_UP:
			OnPointerUp.Broadcast();
			break;

		default:
			break;
		}

		OnInteraction(eventType, payload);
	}

	void IInteractableUI::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
	}


	void IInteractableUI::SetInteractable(bool value)
	{
		if (value == interactable)
			return;

		interactable = value;

		// "none" stops the element being hit-tested at all, which is what makes the backend skip it for
		// hover and clicks without the component having to filter events itself.
		SetStyle(UIStyleProperty::POINTER_EVENTS, value ? "auto" : "none");

		// Mirrored onto a class rather than relying on the backend's own disabled state, so a stylesheet
		// can select it on any element kind, not just the ones with native disabled support.
		SetClass("disabled", !value);
	}

	bool IInteractableUI::IsInteractable() const
	{
		return interactable;
	}

	bool IInteractableUI::IsHovered() const
	{
		return HasState(UIPseudoClass::HOVER);
	}

	bool IInteractableUI::IsPressed() const
	{
		return HasState(UIPseudoClass::ACTIVE);
	}

	bool IInteractableUI::IsFocused() const
	{
		return HasState(UIPseudoClass::FOCUS);
	}
}