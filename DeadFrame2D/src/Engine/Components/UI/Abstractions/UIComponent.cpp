#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	UIComponent::UIComponent()
		: anchor(UIAnchor::CENTER),
		widgetSize(Vector2F::One)
	{
	}

	void UIComponent::Init()
	{
		transform = Guard::AgainstNullAssignment(OwningObject.lock()->GetComponent<Transform>(), NAME_OF(transform));
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
}