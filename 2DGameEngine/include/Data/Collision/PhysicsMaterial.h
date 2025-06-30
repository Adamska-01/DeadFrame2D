#pragma once
#include "Data/Collision/FilterData.h"
#include <box2d/box2d.h>


class b2Shape;
class b2PolygonShape;


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