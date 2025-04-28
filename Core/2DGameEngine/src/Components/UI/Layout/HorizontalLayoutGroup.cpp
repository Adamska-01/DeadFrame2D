#include "Components/Transform.h"
#include "Components/UI/Abstractions/UIComponent.h"
#include "Components/UI/Layout/HorizontalLayoutGroup.h"
#include "GameObject.h"


HorizontalLayoutGroup::HorizontalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
	: LayoutGroup(layoutSpacing, layoutPadding)
{
}

void HorizontalLayoutGroup::UpdateLayout()
{
	auto interactables = OwningObject->GetComponentsInChildren<UIComponent>();

	size_t groupSize = interactables.size();
	if (groupSize == 0)
		return;

	// Precompute total layout width
	float totalWidth = 0.0f;
	for (const auto& interactable : interactables)
	{
		totalWidth += interactable->GetWidgetSize().x;
	}
	totalWidth += layoutSpacing * (groupSize - 1);

	// Get half of the width of the first element
	float firstElementHalfWidth = interactables.front()->GetWidgetSize().x / 2.0f;

	// Start centered
	Vector2F currentPosition{ -totalWidth / 2.0f + firstElementHalfWidth, 0.0f };

	for (auto& interactable : interactables)
	{
		auto transform = interactable->GetGameObject()->GetComponent<Transform>();
		transform->SetLocalPosition(currentPosition);

		currentPosition.x += interactable->GetWidgetSize().x + layoutSpacing;
	}

	isDirty = false;
}