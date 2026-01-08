#pragma once
#include "Data/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct CircleBatchRenderData
	{
		std::vector<CircleRenderData> circleBatch;
	};
}