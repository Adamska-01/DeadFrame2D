#include "Constants/MathConstants.h"
#include "Core/Math/Circle.h"
#include <doctest.h>


using namespace DF2D::Constants;
using namespace DF2D::Core;


TEST_SUITE_BEGIN("Circle");


TEST_CASE("Default circle sits at the origin with radius 1")
{
	Circle circle;

	CHECK(circle.position == Vector2F::Zero);
	CHECK(circle.radius == doctest::Approx(1.0f));
}


TEST_CASE("Constructor stores position and radius")
{
	auto circle = Circle(Vector2F(3.0f, 4.0f), 2.5f);

	CHECK(circle.position == Vector2F(3.0f, 4.0f));
	CHECK(circle.radius == doctest::Approx(2.5f));
}


TEST_CASE("Area is pi r squared")
{
	auto circle = Circle(Vector2F::Zero, 2.0f);

	CHECK(circle.Area() == doctest::Approx(MathConstants::PI * 4.0));
}


TEST_CASE("Circumference is two pi r")
{
	auto circle = Circle(Vector2F::Zero, 2.0f);

	CHECK(circle.Circumference() == doctest::Approx(2.0 * MathConstants::PI * 2.0));
}


TEST_CASE("Area and circumference scale with radius")
{
	auto unit = Circle(Vector2F::Zero, 1.0f);
	auto doubled = Circle(Vector2F::Zero, 2.0f);

	CHECK(doubled.Area() == doctest::Approx(unit.Area() * 4.0));
	CHECK(doubled.Circumference() == doctest::Approx(unit.Circumference() * 2.0));
}


TEST_SUITE_END();