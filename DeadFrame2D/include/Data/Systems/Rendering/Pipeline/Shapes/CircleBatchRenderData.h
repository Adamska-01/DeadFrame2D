#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include <vector>


namespace DF2D::Data
{
	struct CircleBatchRenderData
	{
		std::vector<CircleRenderData> circleBatch;
	};
}