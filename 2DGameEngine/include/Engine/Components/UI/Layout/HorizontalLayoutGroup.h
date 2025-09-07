#pragma once
#include "Data/UI/LayoutPadding.h"
#include "Engine/Components/UI/Abstractions/LayoutGroup.h"


class HorizontalLayoutGroup : public LayoutGroup
{
public:
	HorizontalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding);


	virtual void UpdateLayout() override;
};