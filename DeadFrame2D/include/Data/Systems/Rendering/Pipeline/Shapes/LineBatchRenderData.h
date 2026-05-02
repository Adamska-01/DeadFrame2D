#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/LineRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct LineBatchRenderData
	{
		std::vector<LineRenderData> lineBatch;
	};
}