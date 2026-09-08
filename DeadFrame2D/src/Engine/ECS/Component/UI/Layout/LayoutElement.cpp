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

		// The rect transform may have run first, before this component existed, and written the size
		// properties this one now writes.
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

		// The rect transform writes the position, so this only records the flag and makes it re-apply.
		// Writing the position here would mean two writers again.
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

		// A negative component means "unset", not zero: the axis is left to the content.
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