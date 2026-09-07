#include "Engine/ECS/Component/UI/Layout/LayoutGroup.h"
#include "Utilities/Helpers/UI/StyleValues.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	LayoutGroup::LayoutGroup()
		: spacing(0.0f)
	{
	}


	void LayoutGroup::OnElementCreated()
	{
		ApplyArrangement();
		ApplySpacing();
		ApplyPadding();
	}


	void LayoutGroup::SetSpacing(float value)
	{
		if (value == spacing)
			return;

		spacing = value;

		ApplySpacing();
	}

	void LayoutGroup::SetPadding(const LayoutPadding& value)
	{
		padding = value;

		ApplyPadding();
	}


	float LayoutGroup::GetSpacing() const
	{
		return spacing;
	}

	const LayoutPadding& LayoutGroup::GetPadding() const
	{
		return padding;
	}


	void LayoutGroup::ApplySpacing()
	{
		// Both axes, so the same value serves whichever direction the derived group arranges in, and
		// keeps working when a grid wraps onto a second line. A gap rather than margins on each child:
		// a margin would also appear outside the first and last, which is padding's job, not spacing's.
		SetStyle(UIStyleProperty::ROW_GAP, StyleValues::ToPixels(spacing));
		SetStyle(UIStyleProperty::COLUMN_GAP, StyleValues::ToPixels(spacing));
	}

	void LayoutGroup::ApplyPadding()
	{
		SetStyle(UIStyleProperty::PADDING_LEFT, StyleValues::ToPixels(padding.left));
		SetStyle(UIStyleProperty::PADDING_RIGHT, StyleValues::ToPixels(padding.right));
		SetStyle(UIStyleProperty::PADDING_TOP, StyleValues::ToPixels(padding.top));
		SetStyle(UIStyleProperty::PADDING_BOTTOM, StyleValues::ToPixels(padding.bottom));
	}
}