#pragma once
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include <cstdint>


namespace DF2D::Data
{
	struct FilterData
	{
		uint16_t categoryBits = Core::PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("DEFAULT");

		uint16_t maskBits = Core::PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("ALL");

		int16_t groupIndex = 0;
	};
}