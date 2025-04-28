#pragma once
#include "Components/UI/Abstractions/LayoutGroup.h"
#include "Data/UI/LayoutPadding.h"


class HorizontalLayoutGroup : public LayoutGroup
{
public:
	HorizontalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding);


	virtual void UpdateLayout() override;
};