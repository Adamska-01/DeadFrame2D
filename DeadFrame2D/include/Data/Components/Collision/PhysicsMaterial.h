#pragma once
#include "Data/Components/Collision/FilterData.h"
#include "Data/Components/Collision/Shapes/ShapeDefinition2D.h"


namespace DF2D::Data
{
	struct PhysicsMaterial
	{
		ShapeDefinition2D shape = BoxShapeDefinition2D{};

		float friction = 0.3f;

		float restitution = 0.0f;

		float restitutionThreshold = 1.0f;

		float density = 1.0f;

		bool isSensor = false;

		FilterData filter{};
	};
}