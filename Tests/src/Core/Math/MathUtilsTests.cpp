#include "Core/Math/MathUtils.h"
#include <doctest.h>


using namespace DF2D::Core;


TEST_SUITE_BEGIN("MathUtils");


TEST_CASE("Lerp returns the start value at t equal to zero")
{
	CHECK(MathUtils::Lerp(3.0f, 9.0f, 0.0f) == doctest::Approx(3.0f));
}


TEST_CASE("Lerp returns the end value at t equal to one")
{
	CHECK(MathUtils::Lerp(3.0f, 9.0f, 1.0f) == doctest::Approx(9.0f));
}


TEST_CASE("Lerp returns the midpoint at t equal to one half")
{
	CHECK(MathUtils::Lerp(3.0f, 9.0f, 0.5f) == doctest::Approx(6.0f));
}


TEST_CASE("Lerp extrapolates when t lies outside the zero to one range")
{
	CHECK(MathUtils::Lerp(0.0f, 10.0f, 2.0f) == doctest::Approx(20.0f));
	CHECK(MathUtils::Lerp(0.0f, 10.0f, -1.0f) == doctest::Approx(-10.0f));
}


TEST_CASE("Clamp constrains a value to the inclusive range")
{
	CHECK(MathUtils::Clamp(-5.0f, 0.0f, 1.0f) == doctest::Approx(0.0f));
	CHECK(MathUtils::Clamp(5.0f, 0.0f, 1.0f) == doctest::Approx(1.0f));
	CHECK(MathUtils::Clamp(0.25f, 0.0f, 1.0f) == doctest::Approx(0.25f));
}


TEST_SUITE_END();