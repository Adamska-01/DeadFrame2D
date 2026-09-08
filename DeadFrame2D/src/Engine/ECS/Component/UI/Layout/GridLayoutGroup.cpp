#include "Engine/ECS/Component/UI/Layout/GridLayoutGroup.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	GridLayoutGroup::GridLayoutGroup()
	{
	}


	void GridLayoutGroup::ApplyArrangement()
	{
		SetStyle(UIStyleProperty::DISPLAY, "flex");
		SetStyle(UIStyleProperty::FLEX_DIRECTION, flow == GridFlow::ROWS ? "row" : "column");
		SetStyle(UIStyleProperty::FLEX_WRAP, "wrap");
	}

	void GridLayoutGroup::SetFlow(GridFlow value)
	{
		if (value == flow)
			return;

		flow = value;

		ApplyArrangement();
	}

	GridFlow GridLayoutGroup::GetFlow() const
	{
		return flow;
	}
}