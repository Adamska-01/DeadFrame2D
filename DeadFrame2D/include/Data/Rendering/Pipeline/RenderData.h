#pragma once
#include "Data/Rendering/Pipeline/Shapes/CircleBatchRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/LineBatchRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/PointRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/RectRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "Data/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include <variant>


namespace DeadFrame2D::Data
{
	using RenderData = std::variant<
		SpriteRenderData,
		RectRenderData,
		LineRenderData,
		CircleRenderData,
		PointRenderData,
		PointBatchRenderData,
		LineBatchRenderData,
		CircleBatchRenderData,
		SpriteBatchRenderData>;
}