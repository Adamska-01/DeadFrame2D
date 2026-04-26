#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Abstractions/UIComponent.h"
#include "Engine/ECS/Component/UI/Layout/HorizontalLayoutGroup.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;


	HorizontalLayoutGroup::HorizontalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
		: LayoutGroup(layoutSpacing, layoutPadding)
	{
	}

	void HorizontalLayoutGroup::UpdateLayout()
	{
		LayoutGroup::UpdateLayout();

		std::vector<ComponentHandle<UIComponent>> interactables;

		auto gameObject = GetGameObject();
		for (const auto& ui : gameObject->GetComponentsInChildren<UIComponent>())
		{
			if (ui == nullptr || !ui->GetGameObject()->IsActive())
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
			auto transform = interactable->GetGameObject()->GetTransform();
			transform->SetLocalPosition(currentPosition);

			currentPosition.x += interactable->GetWidgetSize().x + layoutSpacing;
		}
	}
}