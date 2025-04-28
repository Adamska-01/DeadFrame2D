#pragma once
#include "Data/UI/LayoutPadding.h"
#include "Components/UI/Abstractions/LayoutGroup.h"


class VerticalLayoutGroup : public LayoutGroup
{
public:
	VerticalLayoutGroup(float layoutSpacing, LayoutPadding layoutPadding);


	virtual void UpdateLayout() override;
};