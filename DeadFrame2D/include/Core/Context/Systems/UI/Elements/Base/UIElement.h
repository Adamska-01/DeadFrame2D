#pragma once
#include "Core/Context/Systems/UI/Elements/UIDropdown.h"
#include "Core/Context/Systems/UI/Elements/UIScrollable.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/UI/UIAttribute.h"
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIPseudoClass.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <string>


namespace DF2D::Engine
{
	class UIComponent;
}


namespace DF2D::Core
{
	class UIManager;


	/**
	 * @brief A handle to one UI element, carrying everything that can be done to it.
	 */
	class DF2D_API UIElement
	{
		friend class UIContext;
		friend class UIManager;


	private:
		UIManager* manager = nullptr;

		Data::UIElementID id = 0;


		UIElement(UIManager* manager, Data::UIElementID id);


	public:
		UIElement() = default;


		bool operator==(const UIElement& other) const;

		bool operator!=(const UIElement& other) const;


		bool IsValid() const;

		Data::UIElementID Id() const;

		UIDropdown AsDropdown() const;

		UIScrollable AsScrollable() const;


		void SetProperty(Data::UIStyleProperty property, const std::string& value);

		void ClearProperty(Data::UIStyleProperty property);

		void SetAttribute(Data::UIAttribute attribute, const std::string& value);

		void RemoveAttribute(Data::UIAttribute attribute);

		void SetText(const std::string& text);

		void SetClass(const std::string& className, bool enabled);

		bool HasClass(const std::string& className) const;

		bool HasPseudoClass(Data::UIPseudoClass pseudoClass) const;

		void SetVisible(bool visible);

		void SetParent(const UIElement& parent, int siblingIndex);

		RectF GetRect() const;

		Vector2F GetContentSize() const;

		void RegisterOwner(const Engine::ComponentHandle<Engine::UIComponent>& owner);

		void UnregisterOwner(const Engine::ComponentHandleBase& owner);
	};
}