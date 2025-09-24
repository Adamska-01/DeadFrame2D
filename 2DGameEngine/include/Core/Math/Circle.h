#pragma once
#include "Core/Math/Vector2.h"


namespace DeadFrame2D::Core
{
	struct Circle
	{
		Vector2F position;

		float radius;

	
		Circle();

		Circle(const Vector2F& pos, float r);


		float Area() const;

		float Circumference() const;
	};
}