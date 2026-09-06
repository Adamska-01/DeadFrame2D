#include "Core/Context/Systems/UI/UIManager.h"
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
		if (uiManager == nullptr || element == 0)
			return;

		// The stored handle is used rather than asking for a fresh one: resolving a handle runs a type
		// check against an object that is already being destroyed.
		uiManager->UnregisterElementOwner(element, selfUIHandle);

		// Released rather than destroyed: the element is shared with any other UI component on this
		// same GameObject, and only goes away once the last of them lets go.
		uiManager->ReleaseElement(GetGameObject().operator->());
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

	IUIBackend* UIComponent::Backend() const
	{
		return uiManager != nullptr ? &uiManager->Backend() : nullptr;
	}

	void UIComponent::SetStyle(UIStyleProperty property, const std::string& value)
	{
		if (auto* backend = Backend())
		{
			backend->SetElementProperty(element, property, value);
		}
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
		if (auto* backend = Backend())
		{
			backend->ClearElementProperty(element, property);
		}
	}


	void UIComponent::Init()
	{
		uiManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().uiManager, NAME_OF(uiManager));

		// includeSelf, because a Canvas is itself a UIComponent and is its own canvas.
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		Guard::AgainstNull(parentCanvas, NAME_OF(parentCanvas));

		element = parentCanvas->AcquireElementFor(GetGameObject(), GetElementType());

		if (element == 0)
			return;

		selfUIHandle = GetHandleAs<UIComponent>();

		uiManager->RegisterElementOwner(element, selfUIHandle);

		SyncElementParent();

		for (const auto& className : pendingClasses)
		{
			Backend()->SetElementClass(element, className, true);
		}

		pendingClasses.clear();

		OnElementCreated();
	}

	void UIComponent::SyncElementParent()
	{
		auto* backend = Backend();

		if (backend == nullptr || element == 0 || parentCanvas == nullptr)
			return;

		// A canvas is the root of its own tree and has nothing above it to attach to.
		if (element == parentCanvas->GetElement())
			return;

		auto ancestor = GetGameObject()->GetComponentInParent<UIComponent>(true, false);

		auto parentElement = ancestor != nullptr
			? ancestor->GetElement()
			: parentCanvas->GetElement();

		if (parentElement == 0 || parentElement == element)
			return;

		backend->SetElementParent(element, parentElement, -1);
	}

	void UIComponent::OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj)
	{
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		SyncElementParent();

		MarkDirty();
	}

	void UIComponent::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool activeState)
	{
		if (auto* backend = Backend())
		{
			backend->SetElementVisible(element, activeState);
		}
	}


	ComponentHandle<Canvas> UIComponent::GetCanvas() const
	{
		return parentCanvas;
	}

	UIElementID UIComponent::GetElement() const
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
		if (Backend() == nullptr)
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

		Backend()->SetElementClass(element, name, enabled);
	}

	bool UIComponent::HasClass(std::string_view className) const
	{
		auto* backend = Backend();

		return backend != nullptr && backend->HasElementClass(element, std::string(className));
	}

	bool UIComponent::HasState(UIPseudoClass state) const
	{
		auto* backend = Backend();

		return backend != nullptr && backend->HasPseudoClass(element, state);
	}

	void UIComponent::SetStyleProperty(UIStyleProperty property, const std::string& value)
	{
		SetStyle(property, value);
	}

	Core::RectF UIComponent::GetScreenRect() const
	{
		auto* backend = Backend();

		return backend != nullptr ? backend->GetElementRect(element) : Core::RectF{};
	}
}