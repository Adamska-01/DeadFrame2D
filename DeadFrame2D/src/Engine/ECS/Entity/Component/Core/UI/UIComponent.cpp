#include "Core/Context/Systems/UI/UIContext.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/UI/StyleValues.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	UIComponent::UIComponent()
	{
	}

	UIComponent::~UIComponent()
	{
		if (!element.IsValid())
			return;

		// The stored handle is used rather than asking for a fresh one: resolving a handle runs a type
		// check against an object that is already being destroyed.
		element.UnregisterOwner(selfUIHandle);

		// Released rather than destroyed: the element is shared with any other UI component on this same
		// GameObject, and only goes away once the last of them lets go. Released through the surface
		// handle rather than the canvas, which may already be gone by the time this runs.
		context.ReleaseElement(GetGameObject());
	}


	Data::UIElementType UIComponent::GetElementType() const
	{
		return UIElementType::PANEL;
	}

	void UIComponent::OnElementCreated()
	{
	}

	void UIComponent::HandleUIEvent(UIEventType eventType, const UIEventPayload& payload)
	{
	}

	void UIComponent::SetAttribute(UIAttribute attribute, const std::string& value)
	{
		element.SetAttribute(attribute, value);
	}

	void UIComponent::RemoveAttribute(UIAttribute attribute)
	{
		element.RemoveAttribute(attribute);
	}

	void UIComponent::SetElementText(const std::string& text)
	{
		element.SetText(text);
	}

	void UIComponent::SetStyle(UIStyleProperty property, const std::string& value)
	{
		element.SetProperty(property, value);
	}

	void UIComponent::SetStyle(UIStyleProperty property, float pixels)
	{
		SetStyle(property, Utilities::StyleValues::ToPixels(pixels));
	}

	void UIComponent::SetStyle(UIStyleProperty property, const Core::Color& color)
	{
		SetStyle(property, Utilities::StyleValues::ToColor(color));
	}

	void UIComponent::ClearStyle(UIStyleProperty property)
	{
		element.ClearProperty(property);
	}


	void UIComponent::Init()
	{
		// includeSelf, because a Canvas is itself a UIComponent and is its own canvas.
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		Guard::AgainstNull(parentCanvas, NAME_OF(parentCanvas));

		// Copied rather than reached through the canvas on demand: this outlives the canvas component
		// during teardown, and releasing the element is the last thing this component does.
		context = parentCanvas->GetContext();

		// Only declared here, not created. Components on one object are initialised in an order the
		// engine does not define, so the element is built in Start once every component has said what
		// kind it needs.
		parentCanvas->DeclareElementFor(GetGameObject(), GetElementType());
	}

	void UIComponent::Start()
	{
		if (parentCanvas == nullptr || element.IsValid())
			return;

		element = parentCanvas->AcquireElementFor(GetGameObject());

		if (!element.IsValid())
			return;

		selfUIHandle = GetHandleAs<UIComponent>();

		element.RegisterOwner(selfUIHandle);

		SyncElementParent();

		for (const auto& className : pendingClasses)
		{
			element.SetClass(className, true);
		}

		pendingClasses.clear();

		OnElementCreated();
	}

	void UIComponent::SyncElementParent()
	{
		if (!element.IsValid() || parentCanvas == nullptr)
			return;

		// A canvas is the root of its own tree and has nothing above it to attach to.
		if (element == parentCanvas->GetElement())
			return;

		auto ancestor = GetGameObject()->GetComponentInParent<UIComponent>(true, false);

		auto parentElement = ancestor != nullptr
			? ancestor->GetElement()
			: parentCanvas->GetElement();

		if (!parentElement.IsValid() || parentElement == element)
			return;

		element.SetParent(parentElement, -1);
	}

	void UIComponent::OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj)
	{
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		SyncElementParent();

		MarkDirty();
	}

	void UIComponent::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool activeState)
	{
		element.SetVisible(activeState);
	}


	ComponentHandle<Canvas> UIComponent::GetCanvas() const
	{
		return parentCanvas;
	}

	UIElement UIComponent::GetElement() const
	{
		return element;
	}

	void UIComponent::AddClass(std::string_view className)
	{
		SetClass(className, true);
	}

	void UIComponent::RemoveClass(std::string_view className)
	{
		SetClass(className, false);
	}

	void UIComponent::SetClass(std::string_view className, bool enabled)
	{
		auto name = std::string(className);

		// Scenes are built in Enter, which runs before any component is initialised, so there is no
		// element to style yet. The intent is recorded and replayed once there is, the same way
		// PlayerInput replays action registrations made before its own Init.
		//
		// Keyed on the element rather than the manager: the manager is resolved in Init but the element
		// only exists after Start, and a class set in between would otherwise be written to element 0
		// and silently lost.
		if (!element.IsValid())
		{
			if (enabled)
			{
				pendingClasses.insert(std::move(name));
			}
			else
			{
				pendingClasses.erase(name);
			}

			return;
		}

		element.SetClass(name, enabled);
	}

	bool UIComponent::HasClass(std::string_view className) const
	{
		return element.HasClass(std::string(className));
	}

	bool UIComponent::HasState(UIPseudoClass state) const
	{
		return element.HasPseudoClass(state);
	}

	void UIComponent::SetStyleProperty(UIStyleProperty property, const std::string& value)
	{
		SetStyle(property, value);
	}

	Core::RectF UIComponent::GetScreenRect() const
	{
		return element.GetRect();
	}
}