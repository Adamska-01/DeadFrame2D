#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include <vector>


namespace DF2D::Data
{
	struct SpriteBatchRenderData
	{
		std::vector<SpriteRenderData> spriteBatch;
	};
}