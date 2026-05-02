#pragma once
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/LineBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/RectRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteRenderData.h"
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