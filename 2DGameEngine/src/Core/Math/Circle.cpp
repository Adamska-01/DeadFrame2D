#include "Constants/MathConstants.h"
#include "Core/Math/Circle.h"
#include "Core/Math/Vector2.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;


	Circle::Circle()
		: position(Vector2F::Zero), radius(1.0f)
	{
	}

	Circle::Circle(const Vector2F& pos, float r) 
		: position(pos), radius(r) 
	{
	}

	float Circle::Area() const 
	{
		return MathConstants::PI * radius * radius;
	}

	float Circle::Circumference() const 
	{
		return 2.0f * MathConstants::PI * radius;
	}
}