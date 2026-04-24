#pragma once
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include <cstdint>


namespace DeadFrame2D::Data
{
	struct FilterData
	{
		uint16_t categoryBits = DeadFrame2D::Core::PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("DEFAULT");

		uint16_t maskBits = DeadFrame2D::Core::PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("ALL");

		int16_t groupIndex = 0;
	};
}