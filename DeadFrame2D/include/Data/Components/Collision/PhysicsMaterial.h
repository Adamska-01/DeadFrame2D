#pragma once
#include "Data/Components/Collision/FilterData.h"
#include <box2d/box2d.h>


namespace DeadFrame2D::Data
{
	struct PhysicsMaterial
	{
		b2Shape* shape = new b2PolygonShape();

		float friction = 0.3f;
	
		float restitution = 0.0f;
	
		float restitutionThreshold = 1.0f;
	
		float density = 1.0f;
	
		bool isSensor = false;
	
		FilterData filter{};
	};
}