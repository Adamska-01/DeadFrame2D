#include "Engine/ECS/Component/UI/Layout/ContentSizeFitter.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	ContentSizeFitter::ContentSizeFitter()
	{
	}


	void ContentSizeFitter::OnElementCreated()
	{
		// The rect transform may have run first, before this component existed, and sized the element
		// from sizeDelta.
		RefreshPlacement();
	}


	void ContentSizeFitter::SetHorizontalFit(SizeFitMode value)
	{
		if (value == horizontalFit)
			return;

		horizontalFit = value;

		RefreshPlacement();
	}

	void ContentSizeFitter::SetVerticalFit(SizeFitMode value)
	{
		if (value == verticalFit)
			return;

		verticalFit = value;

		RefreshPlacement();
	}


	SizeFitMode ContentSizeFitter::GetHorizontalFit() const
	{
		return horizontalFit;
	}

	SizeFitMode ContentSizeFitter::GetVerticalFit() const
	{
		return verticalFit;
	}


	void ContentSizeFitter::RefreshPlacement()
	{
		if (auto rectTransform = GetGameObject()->GetComponent<RectTransform>())
		{
			rectTransform->RefreshPlacement();
		}
	}
}