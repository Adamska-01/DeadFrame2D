#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	struct LineRenderData
	{
		Core::Vector2F p1 = Core::Vector2F::Zero;

		Core::Vector2F p2 = Core::Vector2F::Zero;

		Core::Color color = Constants::CommonColors::WHITE;
	};
}