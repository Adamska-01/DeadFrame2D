#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/LineRenderData.h"
#include <vector>


namespace DF2D::Data
{
	struct LineBatchRenderData
	{
		std::vector<LineRenderData> lineBatch;
	};
}