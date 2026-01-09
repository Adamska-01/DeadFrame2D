#pragma once
#include "Data/Rendering/Pipeline/Shapes/RectRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct RectBatchRenderData
	{
		std::vector<RectRenderData> rectBatch;
	};
}