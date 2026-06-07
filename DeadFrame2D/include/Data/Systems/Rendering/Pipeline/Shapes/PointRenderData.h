#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	struct PointRenderData
	{
		Core::Vector2F pos = Core::Vector2F::Zero;

		Core::Color color = Constants::CommonColors::WHITE;
	};
}