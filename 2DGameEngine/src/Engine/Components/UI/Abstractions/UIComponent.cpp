#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


UIComponent::UIComponent()
	: anchor(UIAnchor::CENTER),
	widgetSize(Vector2F::One),
	transform(nullptr)
{
}

void UIComponent::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();

	Tools::Helpers::GuardAgainstNull(transform, "Failed to get Transform from OwningObject");
}

void UIComponent::SetAnchor(UIAnchor newAnchor)
{
	anchor = newAnchor;
}

void UIComponent::SetWidgetSize(Vector2F newWidgetSize)
{
	widgetSize = newWidgetSize;
}

Vector2F UIComponent::GetWidgetSize() const
{
	return widgetSize * transform->GetWorldScale();
}