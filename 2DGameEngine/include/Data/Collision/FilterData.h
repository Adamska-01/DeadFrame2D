#pragma once
#include "SubSystems/Physics/PhysicsEngine2D.h"
#include <cstdint>


struct FilterData
{
	uint16_t categoryBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("DEFAULT");

	uint16_t maskBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("ALL");

	int16_t groupIndex = 0;
};