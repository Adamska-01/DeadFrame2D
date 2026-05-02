#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/PointRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct PointBatchRenderData
	{
		std::vector<PointRenderData> pointBatch;
	};
}