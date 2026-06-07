#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"


namespace DF2D::Data
{
	struct RectRenderData
	{
		bool filled = true;

		Core::RectF destRect = Core::RectF{ 0.0f, 0.0f, 0.0f, 0.0f };

		float rotation = 0.0f;

		Core::Color color = Constants::CommonColors::WHITE;
	};
}