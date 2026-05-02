#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct CircleBatchRenderData
	{
		std::vector<CircleRenderData> circleBatch;
	};
}