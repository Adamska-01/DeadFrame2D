#include "Core/Context/Systems/UI/Elements/Base/UIElement.h"
#include "Core/Context/Systems/UI/UIManager.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;


	UIElement::UIElement(UIManager* manager, UIElementID id)
		: manager(manager),
		id(id)
	{
	}


	bool UIElement::operator==(const UIElement& other) const
	{
		return manager == other.manager && id == other.id;
	}

	bool UIElement::operator!=(const UIElement& other) const
	{
		return !(*this == other);
	}


	bool UIElement::IsValid() const
	{
		return manager != nullptr && id != 0;
	}

	UIElementID UIElement::Id() const
	{
		return id;
	}

	UIDropdown UIElement::AsDropdown() const
	{
		return UIDropdown(manager, id);
	}

	UIScrollable UIElement::AsScrollable() const
	{
		return UIScrollable(manager, id);
	}


	void UIElement::SetProperty(UIStyleProperty property, const std::string& value)
	{
		if (IsValid())
		{
			manager->SetElementProperty(id, property, value);
		}
	}

	void UIElement::ClearProperty(UIStyleProperty property)
	{
		if (IsValid())
		{
			manager->ClearElementProperty(id, property);
		}
	}

	void UIElement::SetAttribute(UIAttribute attribute, const std::string& value)
	{
		if (IsValid())
		{
			manager->SetElementAttribute(id, attribute, value);
		}
	}

	void UIElement::RemoveAttribute(UIAttribute attribute)
	{
		if (IsValid())
		{
			manager->RemoveElementAttribute(id, attribute);
		}
	}

	void UIElement::SetText(const std::string& text)
	{
		if (IsValid())
		{
			manager->SetElementText(id, text);
		}
	}

	void UIElement::SetClass(const std::string& className, bool enabled)
	{
		if (IsValid())
		{
			manager->SetElementClass(id, className, enabled);
		}
	}

	bool UIElement::HasClass(const std::string& className) const
	{
		return IsValid() && manager->HasElementClass(id, className);
	}

	bool UIElement::HasPseudoClass(UIPseudoClass pseudoClass) const
	{
		return IsValid() && manager->HasPseudoClass(id, pseudoClass);
	}

	void UIElement::SetVisible(bool visible)
	{
		if (IsValid())
		{
			manager->SetElementVisible(id, visible);
		}
	}

	void UIElement::SetParent(const UIElement& parent, int siblingIndex)
	{
		if (IsValid() && parent.IsValid())
		{
			manager->SetElementParent(id, parent.id, siblingIndex);
		}
	}

	RectF UIElement::GetRect() const
	{
		return IsValid() ? manager->GetElementRect(id) : RectF{};
	}

	Vector2F UIElement::GetContentSize() const
	{
		return IsValid() ? manager->GetElementContentSize(id) : Vector2F::Zero;
	}

	void UIElement::RegisterOwner(const Engine::ComponentHandle<Engine::UIComponent>& owner)
	{
		if (IsValid())
		{
			manager->RegisterElementOwner(id, owner);
		}
	}

	void UIElement::UnregisterOwner(const Engine::ComponentHandleBase& owner)
	{
		if (IsValid())
		{
			manager->UnregisterElementOwner(id, owner);
		}
	}
}