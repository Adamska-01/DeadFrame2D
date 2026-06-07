#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	struct CircleRenderData
	{
		Core::Vector2F center = Core::Vector2F::Zero;

		float radius = 1.0f;

		bool filled = true;

		Core::Color color = Constants::CommonColors::WHITE;
	};
}