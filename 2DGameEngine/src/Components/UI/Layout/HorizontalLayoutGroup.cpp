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

	// Precompute total layout width
	auto totalWidth = 0.0f;
	for (const auto& interactable : interactables)
	{
		totalWidth += interactable->GetWidgetSize().x;
	}
	totalWidth += layoutSpacing * (groupSize - 1);

	// Get half of the width of the first element
	auto firstElementHalfWidth = interactables.front()->GetWidgetSize().x / 2.0f;

	// Start centered
	auto currentPosition = Vector2F(- totalWidth / 2.0f + firstElementHalfWidth, 0.0f);

	for (auto& interactable : interactables)
	{
		auto transform = interactable->GetGameObject().lock()->GetComponent<Transform>();
		transform->SetLocalPosition(currentPosition);

		currentPosition.x += interactable->GetWidgetSize().x + layoutSpacing;
	}

	isDirty = false;
}