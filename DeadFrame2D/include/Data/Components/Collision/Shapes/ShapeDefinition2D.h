#pragma once
#include "Data/Components/Collision/Shapes/Definitions/BoxShapeDefinition2D.h"
#include "Data/Components/Collision/Shapes/Definitions/CircleShapeDefinition2D.h"
#include <variant>


namespace DF2D::Data
{
	using ShapeDefinition2D = std::variant<
		BoxShapeDefinition2D,
		CircleShapeDefinition2D>;
}