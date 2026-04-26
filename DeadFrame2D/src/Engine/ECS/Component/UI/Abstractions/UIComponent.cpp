#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Abstractions/UIComponent.h"
#include "Engine/ECS/Component/UI/Canvas.h"
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
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		MarkDirty();
	}
	
	void UIComponent::Update(float deltaTime)
	{
		if (isDirty)
		{
			Guard::AgainstNull(parentCanvas, NAME_OF(parentCanvas));
		}
	}

	void UIComponent::OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj)
	{
		parentCanvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		MarkDirty();
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