#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/RectRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct RectBatchRenderData
	{
		std::vector<RectRenderData> rectBatch;
	};
}