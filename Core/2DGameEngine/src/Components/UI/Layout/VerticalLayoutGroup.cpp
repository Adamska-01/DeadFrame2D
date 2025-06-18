#include "Components/Transform.h"
#include "Components/UI/Abstractions/UIComponent.h"
#include "Components/UI/Layout/VerticalLayoutGroup.h"
#include "GameObject.h"


VerticalLayoutGroup::VerticalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
	: LayoutGroup(layoutSpacing, layoutPadding)
{
}

void VerticalLayoutGroup::UpdateLayout()
{
	LayoutGroup::UpdateLayout();

	std::vector<UIComponent*> interactables;

	for (const auto& ui : OwningObject.lock()->GetComponentsInChildren<UIComponent>())
	{
		if (ui == nullptr || !ui->GetGameObject().lock()->IsActive())
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

	// Start at top (centered layout origin minus half height)
	auto currentPosition = Vector2F
	{
		0.0f,
		-totalHeight / 2.0f
	};

	for (auto& interactable : interactables)
	{
		auto size = interactable->GetWidgetSize();

		auto transform = interactable->GetGameObject().lock()->GetComponent<Transform>();
		
		if (transform == nullptr)
			continue;

		// Set local position to center the widget at current position + half its height
		transform->SetLocalPosition(currentPosition + Vector2F(0.0f, size.y / 2.0f));

		// Move down by full height + spacing
		currentPosition.y += size.y + layoutSpacing;
	}

	isDirty = false;
}