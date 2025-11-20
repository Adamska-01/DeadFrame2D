#pragma once
#include "Data/Components/UI/Button/ButtonComponentModel.h"
#include "Data/Components/UI/TextMeshComponentModel.h"


namespace DeadFrame2D::Data
{
	struct ButtonBlueprintModel
	{
		ButtonComponentModel buttonComponentModel;

		TextMeshComponentModel textMeshComponentModel;
	};
}