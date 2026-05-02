#pragma once
#include "Data/Components/UI/Layout/LayoutPadding.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Layout/Abstractions/LayoutGroup.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API HorizontalLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(HorizontalLayoutGroup, LayoutGroup);


	public:
		HorizontalLayoutGroup(float layoutSpacing, DeadFrame2D::Data::LayoutPadding layoutPadding);


		virtual void UpdateLayout() override;
	};
}