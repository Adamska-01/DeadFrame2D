#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	namespace
	{
		/** @brief Index a direction takes in the per-direction target table. */
		int ToIndex(UINavigationDirection direction)
		{
			return static_cast<int>(direction);
		}

		UIStyleProperty ToNavigationProperty(UINavigationDirection direction)
		{
			switch (direction)
			{
			case UINavigationDirection::UP:		return UIStyleProperty::NAV_UP;
			case UINavigationDirection::DOWN:	return UIStyleProperty::NAV_DOWN;
			case UINavigationDirection::LEFT:	return UIStyleProperty::NAV_LEFT;
			default:							return UIStyleProperty::NAV_RIGHT;
			}
		}

		/** @brief What a mode resolves to on one axis, before explicit targets are laid over it. */
		const char* ToKeyword(UINavigationMode mode, bool horizontal)
		{
			switch (mode)
			{
			case UINavigationMode::AUTOMATIC:	return "auto";
			case UINavigationMode::TREE_ORDER:	return "tree-order";
			case UINavigationMode::HORIZONTAL:	return horizontal ? "auto" : "none";
			case UINavigationMode::VERTICAL:	return horizontal ? "none" : "auto";
			default:							return "none";
			}
		}
	}


	void IInteractableUI::ApplyNavigability()
	{
		// Keyboard focus is what navigation moves around, and what makes Enter activate this element.
		SetStyle(UIStyleProperty::TAB_INDEX, interactable ? "auto" : "none");

		// A widget that refuses every interaction must not be reachable either, whatever its mode says.
		auto mode = interactable ? navigationMode : UINavigationMode::NONE;

		SetStyle(UIStyleProperty::NAV_UP, ToKeyword(mode, false));
		SetStyle(UIStyleProperty::NAV_DOWN, ToKeyword(mode, false));
		SetStyle(UIStyleProperty::NAV_LEFT, ToKeyword(mode, true));
		SetStyle(UIStyleProperty::NAV_RIGHT, ToKeyword(mode, true));

		// Named neighbours are laid over the mode, and only where one was actually named: a direction
		// left unset in EXPLICIT stays put rather than falling back to whatever is nearest.
		if (mode != UINavigationMode::EXPLICIT)
			return;

		for (auto direction : { UINavigationDirection::UP, UINavigationDirection::DOWN,
			UINavigationDirection::LEFT, UINavigationDirection::RIGHT })
		{
			const auto& target = navigationTargets[ToIndex(direction)];

			if (!target.empty())
			{
				SetStyle(ToNavigationProperty(direction), "#" + target);
			}
		}
	}

	const std::string& IInteractableUI::NavigationId()
	{
		// Assigned on first use rather than to everything: an id only matters to a widget something
		// else navigates to, and the rest are better left without one for a stylesheet to use.
		if (navigationId.empty())
		{
			static auto nextId = 0;

			navigationId = "df2d-nav-" + std::to_string(++nextId);

			SetAttribute(UIAttribute::ID, navigationId);
		}

		return navigationId;
	}

	void IInteractableUI::SetNavigationMode(UINavigationMode mode)
	{
		if (mode == navigationMode)
			return;

		navigationMode = mode;

		ApplyNavigability();
	}

	void IInteractableUI::SetNavigationTarget(UINavigationDirection direction, const ComponentHandle<IInteractableUI>& target)
	{
		// Naming a neighbour is only meaningful when this widget's own links are what decide, so saying
		// so switches the mode rather than being quietly ignored under AUTOMATIC.
		navigationMode = UINavigationMode::EXPLICIT;

		navigationTargets[ToIndex(direction)] = target != nullptr ? target->NavigationId() : std::string();

		ApplyNavigability();
	}

	UINavigationMode IInteractableUI::GetNavigationMode() const
	{
		return navigationMode;
	}

	void IInteractableUI::OnElementCreated()
	{
		// The id may have been handed out while something else was wiring up its navigation, before this
		// widget had an element to carry it. Written now, or the neighbour pointing here finds nothing.
		if (!navigationId.empty())
		{
			SetAttribute(UIAttribute::ID, navigationId);
		}

		ApplyNavigability();

		if (focusRequested)
		{
			focusRequested = false;

			element.Focus();
		}

		OnInteractableCreated();
	}

	void IInteractableUI::OnInteractableCreated()
	{
	}

	void IInteractableUI::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
	}

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

		case UIEventType::FOCUS_GAINED:
			OnFocusChanged.Broadcast(true);
			break;

		case UIEventType::FOCUS_LOST:
			OnFocusChanged.Broadcast(false);
			break;

		default:
			break;
		}

		OnInteraction(eventType, payload);
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

		// A disabled widget must also drop out of navigation, or focus would still land on something
		// that refuses every interaction once it gets there.
		ApplyNavigability();
	}

	bool IInteractableUI::IsInteractable() const
	{
		return interactable;
	}

	void IInteractableUI::Focus()
	{
		// Components start in an order the engine does not define, so whoever wants focus first may ask
		// before this widget has an element to give it. The request is kept and taken once there is one,
		// the same way classes and stylesheets are replayed.
		if (!element.IsValid())
		{
			focusRequested = true;

			return;
		}

		element.Focus();
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