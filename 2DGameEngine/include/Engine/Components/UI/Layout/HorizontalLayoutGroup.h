#pragma once
#include "Data/UI/LayoutPadding.h"
#include "Engine/Components/UI/Abstractions/LayoutGroup.h"


namespace DeadFrame2D::Engine
{
	class HorizontalLayoutGroup : public LayoutGroup
	{
	public:
		HorizontalLayoutGroup(float layoutSpacing, DeadFrame2D::Data::LayoutPadding layoutPadding);


		virtual void UpdateLayout() override;
	};
}