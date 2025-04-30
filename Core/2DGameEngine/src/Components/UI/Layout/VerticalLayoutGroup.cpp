#include "Components/Transform.h"
#include "Components/UI/Abstractions/UIComponent.h"
#include "Components/UI/Layout/VerticalLayoutGroup.h"
#include "GameObject.h"
#include "Tools/Helpers/EventHelpers.h"


VerticalLayoutGroup::VerticalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
	: LayoutGroup(layoutSpacing, layoutPadding)
{
}

void VerticalLayoutGroup::UpdateLayout()
{
	LayoutGroup::UpdateLayout();

	std::vector<UIComponent*> interactables;

	for (const auto& ui : OwningObject->GetComponentsInChildren<UIComponent>())
	{
		if (ui == nullptr || !ui->GetGameObject()->IsActive())
			continue;

		interactables.push_back(ui);
	}

	auto groupSize = interactables.size();
	if (groupSize == 0)
		return;

	// Precompute total layout height
	auto totalHeight = 0.0f;
	for (const auto& interactable : interactables)
	{
		totalHeight += interactable->GetWidgetSize().y;
	}
	totalHeight += layoutSpacing * (groupSize - 1);

	// Get half of the height of the first element
	auto firstElementHalfHeight = interactables.front()->GetWidgetSize().y / 2.0f;

	// Start centered
	auto currentPosition = Vector2F
	{ 
		0.0f, 
		-totalHeight / 2.0f + firstElementHalfHeight 
	};

	for (auto& interactable : interactables)
	{
		auto transform = interactable->GetGameObject()->GetComponent<Transform>();
		transform->SetLocalPosition(currentPosition);

		currentPosition.y += interactable->GetWidgetSize().y + layoutSpacing;
	}

	isDirty = false;
}