#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	struct DF2D_API Circle
	{
		Vector2F position;

		float radius;


		Circle();

		Circle(const Vector2F& pos, float r);


		double Area() const;

		double Circumference() const;
	};
}