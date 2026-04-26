#pragma once
#include "Data/UI/LayoutPadding.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Layout/Abstractions/LayoutGroup.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API VerticalLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(VerticalLayoutGroup, LayoutGroup);


	public:
		VerticalLayoutGroup(float layoutSpacing, DeadFrame2D::Data::LayoutPadding layoutPadding);


		virtual void UpdateLayout() override;
	};
}