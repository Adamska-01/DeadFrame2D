#include "Engine/ECS/Component/UI/Layout/HorizontalLayoutGroup.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	HorizontalLayoutGroup::HorizontalLayoutGroup()
	{
	}


	void HorizontalLayoutGroup::ApplyArrangement()
	{
		SetStyle(UIStyleProperty::DISPLAY, "flex");
		SetStyle(UIStyleProperty::FLEX_DIRECTION, "row");
		SetStyle(UIStyleProperty::FLEX_WRAP, "nowrap");
	}
}