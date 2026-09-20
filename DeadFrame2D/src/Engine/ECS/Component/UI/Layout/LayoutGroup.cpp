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
		// Apply the spacing to both axes so it works for rows, columns, and grids.
		// Use a gap instead of child margins so the first and last child are not offset.
		SetStyle(UIStyleProperty::ROW_GAP, StyleValues::ToDp(spacing));
		SetStyle(UIStyleProperty::COLUMN_GAP, StyleValues::ToDp(spacing));
	}

	void LayoutGroup::ApplyPadding()
	{
		// Use dp so the padding scales with the children and the rest of the layout.
		SetStyle(UIStyleProperty::PADDING_LEFT, StyleValues::ToDp(padding.left));
		SetStyle(UIStyleProperty::PADDING_RIGHT, StyleValues::ToDp(padding.right));
		SetStyle(UIStyleProperty::PADDING_TOP, StyleValues::ToDp(padding.top));
		SetStyle(UIStyleProperty::PADDING_BOTTOM, StyleValues::ToDp(padding.bottom));
	}
}