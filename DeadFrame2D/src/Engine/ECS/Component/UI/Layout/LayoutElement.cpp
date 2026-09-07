#include "Engine/ECS/Component/UI/Layout/LayoutElement.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Helpers/UI/StyleValues.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	LayoutElement::LayoutElement()
	{
	}


	void LayoutElement::OnElementCreated()
	{
		ApplyFlex();
		ApplySizes();

		// The rect transform may have resolved before this component existed, in which case it wrote
		// the size properties this one now owns.
		RefreshPlacement();
	}


	void LayoutElement::SetFlexGrow(float value)
	{
		if (value == flexGrow)
			return;

		flexGrow = value;

		ApplyFlex();
	}

	void LayoutElement::SetFlexShrink(float value)
	{
		if (value == flexShrink)
			return;

		flexShrink = value;

		ApplyFlex();
	}

	void LayoutElement::SetMinSize(const Vector2F& value)
	{
		minSize = value;

		ApplySizes();
	}

	void LayoutElement::SetPreferredSize(const Vector2F& value)
	{
		preferredSize = value;

		ApplySizes();
	}

	void LayoutElement::SetIgnoreLayout(bool value)
	{
		if (value == ignoreLayout)
			return;

		ignoreLayout = value;

		// Position belongs to the rect transform, so this only records the intent and asks it to
		// resolve again. Writing the position here would be the same two-owner problem in reverse.
		RefreshPlacement();
	}


	float LayoutElement::GetFlexGrow() const
	{
		return flexGrow;
	}

	float LayoutElement::GetFlexShrink() const
	{
		return flexShrink;
	}

	const Vector2F& LayoutElement::GetMinSize() const
	{
		return minSize;
	}

	const Vector2F& LayoutElement::GetPreferredSize() const
	{
		return preferredSize;
	}

	bool LayoutElement::IsIgnoringLayout() const
	{
		return ignoreLayout;
	}


	void LayoutElement::ApplyFlex()
	{
		SetStyle(UIStyleProperty::FLEX_GROW, StyleValues::ToNumber(flexGrow));
		SetStyle(UIStyleProperty::FLEX_SHRINK, StyleValues::ToNumber(flexShrink));
	}

	void LayoutElement::ApplySizes()
	{
		SetStyle(UIStyleProperty::MIN_WIDTH, StyleValues::ToPixels(minSize.x));
		SetStyle(UIStyleProperty::MIN_HEIGHT, StyleValues::ToPixels(minSize.y));

		// A negative component means "no opinion", which is not the same as zero: the axis is left to
		// the content rather than collapsed.
		if (preferredSize.x >= 0.0f)
		{
			SetStyle(UIStyleProperty::WIDTH, StyleValues::ToPixels(preferredSize.x));
		}
		else
		{
			SetStyle(UIStyleProperty::WIDTH, "auto");
		}

		if (preferredSize.y >= 0.0f)
		{
			SetStyle(UIStyleProperty::HEIGHT, StyleValues::ToPixels(preferredSize.y));
		}
		else
		{
			SetStyle(UIStyleProperty::HEIGHT, "auto");
		}
	}

	void LayoutElement::RefreshPlacement()
	{
		if (auto rectTransform = GetGameObject()->GetComponent<RectTransform>())
		{
			rectTransform->RefreshPlacement();
		}
	}
}
