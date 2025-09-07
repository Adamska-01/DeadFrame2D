#pragma once
#include "Core/Math/Vector2.h"


struct Circle
{
	Vector2F position;

	float radius;

	
	Circle();

	Circle(const Vector2F& pos, float r);


	float Area() const;

	float Circumference() const;
};
