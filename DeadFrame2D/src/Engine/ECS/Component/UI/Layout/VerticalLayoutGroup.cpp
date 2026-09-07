#include "Engine/ECS/Component/UI/Layout/VerticalLayoutGroup.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	VerticalLayoutGroup::VerticalLayoutGroup()
	{
	}


	void VerticalLayoutGroup::ApplyArrangement()
	{
		SetStyle(UIStyleProperty::DISPLAY, "flex");
		SetStyle(UIStyleProperty::FLEX_DIRECTION, "column");
		SetStyle(UIStyleProperty::FLEX_WRAP, "nowrap");
	}
}