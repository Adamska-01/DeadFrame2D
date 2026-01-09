#pragma once
#include "Data/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include <vector>


namespace DeadFrame2D::Data
{
	struct SpriteBatchRenderData
	{
		std::vector<SpriteRenderData> spriteBatch;
	};
}