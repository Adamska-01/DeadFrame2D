#pragma once
#include "../../Math/Vector2.h"


struct Circle
{
	Vector2 position;

	float radius;

	
	Circle();

	Circle(const Vector2& pos, float r);


	float Area() const;

	float Circumference() const;
};
