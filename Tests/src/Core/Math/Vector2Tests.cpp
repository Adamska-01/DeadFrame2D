#include "Core/Math/Vector2.h"
#include <doctest.h>
#include <unordered_set>


using namespace DF2D::Core;


TEST_SUITE_BEGIN("Vector2");


TEST_CASE("Default constructor yields zero vector")
{
	Vector2F v;

	CHECK(v.x == 0.0f);
	CHECK(v.y == 0.0f);
	CHECK(v.IsZero());
}


TEST_CASE("Static constants have the expected values")
{
	CHECK(Vector2F::Zero == Vector2F(0.0f, 0.0f));
	CHECK(Vector2F::One == Vector2F(1.0f, 1.0f));
	CHECK(Vector2F::Up == Vector2F(0.0f, -1.0f));
	CHECK(Vector2F::Down == Vector2F(0.0f, 1.0f));
	CHECK(Vector2F::Left == Vector2F(-1.0f, 0.0f));
	CHECK(Vector2F::Right == Vector2F(1.0f, 0.0f));
}


TEST_CASE("Addition and subtraction are component-wise")
{
	auto a = Vector2F(3.0f, 4.0f);
	auto b = Vector2F(1.0f, 2.0f);

	CHECK(a + b == Vector2F(4.0f, 6.0f));
	CHECK(a - b == Vector2F(2.0f, 2.0f));
}


TEST_CASE("Multiplication and division are component-wise")
{
	auto a = Vector2F(3.0f, 4.0f);
	auto b = Vector2F(2.0f, 4.0f);

	CHECK(a * b == Vector2F(6.0f, 16.0f));
	CHECK(a / b == Vector2F(1.5f, 1.0f));
}


TEST_CASE("Scalar multiplication and division scale both components")
{
	auto v = Vector2F(3.0f, -4.0f);

	CHECK(v * 2.0f == Vector2F(6.0f, -8.0f));
	CHECK(v / 2.0f == Vector2F(1.5f, -2.0f));
}


TEST_CASE("Compound assignment operators mutate in place")
{
	auto v = Vector2F(1.0f, 2.0f);

	v += Vector2F(2.0f, 3.0f);
	CHECK(v == Vector2F(3.0f, 5.0f));

	v -= Vector2F(1.0f, 1.0f);
	CHECK(v == Vector2F(2.0f, 4.0f));

	v *= 2.0f;
	CHECK(v == Vector2F(4.0f, 8.0f));

	v /= 4.0f;
	CHECK(v == Vector2F(1.0f, 2.0f));

	v *= Vector2F(3.0f, 5.0f);
	CHECK(v == Vector2F(3.0f, 10.0f));

	v /= Vector2F(3.0f, 5.0f);
	CHECK(v == Vector2F(1.0f, 2.0f));
}


TEST_CASE("Ordering compares x first, then y")
{
	CHECK(Vector2F(1.0f, 2.0f) < Vector2F(2.0f, 0.0f));
	CHECK(Vector2F(1.0f, 2.0f) < Vector2F(1.0f, 3.0f));
	CHECK_FALSE(Vector2F(2.0f, 0.0f) < Vector2F(1.0f, 2.0f));
	CHECK_FALSE(Vector2F(1.0f, 2.0f) < Vector2F(1.0f, 2.0f));
}


TEST_CASE("Magnitude and Distance follow the 3-4-5 triangle")
{
	CHECK(Vector2F(3.0f, 4.0f).Magnitude() == doctest::Approx(5.0f));
	CHECK(Vector2F(0.0f, 0.0f).Distance(Vector2F(3.0f, 4.0f)) == doctest::Approx(5.0f));
	CHECK(Vector2F(1.0f, 1.0f).Distance(Vector2F(4.0f, 5.0f)) == doctest::Approx(5.0f));
}


TEST_CASE("Dot product, member and static, agree")
{
	auto a = Vector2F(1.0f, 2.0f);
	auto b = Vector2F(3.0f, 4.0f);

	CHECK(a.Dot(b) == doctest::Approx(11.0f));
	CHECK(Vector2F::Dot(a, b) == doctest::Approx(11.0f));

	// Perpendicular vectors dot to zero
	CHECK(Vector2F::Right.Dot(Vector2F::Down) == doctest::Approx(0.0f));
}


TEST_CASE("Cross product returns the 2D scalar cross")
{
	auto a = Vector2F(1.0f, 2.0f);
	auto b = Vector2F(3.0f, 4.0f);

	CHECK(a.Cross(b) == doctest::Approx(-2.0f));
	CHECK(b.Cross(a) == doctest::Approx(2.0f));

	// Parallel vectors cross to zero
	CHECK(a.Cross(a * 3.0f) == doctest::Approx(0.0f));
}


TEST_CASE("Min and Max are component-wise")
{
	auto a = Vector2F(1.0f, 5.0f);
	auto b = Vector2F(3.0f, 2.0f);

	CHECK(a.Min(b) == Vector2F(1.0f, 2.0f));
	CHECK(a.Max(b) == Vector2F(3.0f, 5.0f));
}


TEST_CASE("Rotated rotates by degrees")
{
	auto v = Vector2F(1.0f, 0.0f);

	auto rotated90 = v.Rotated(90.0f);

	CHECK(rotated90.x == doctest::Approx(0.0f));
	CHECK(rotated90.y == doctest::Approx(1.0f));

	auto rotated180 = v.Rotated(180.0f);

	CHECK(rotated180.x == doctest::Approx(-1.0f));
	CHECK(rotated180.y == doctest::Approx(0.0f));

	auto fullCircle = Vector2F(3.0f, 4.0f).Rotated(360.0f);

	CHECK(fullCircle.x == doctest::Approx(3.0f));
	CHECK(fullCircle.y == doctest::Approx(4.0f));
}


TEST_CASE("Normalize produces a unit vector and preserves direction")
{
	auto v = Vector2F(3.0f, 4.0f);

	v.Normalize();

	CHECK(v.x == doctest::Approx(0.6f));
	CHECK(v.y == doctest::Approx(0.8f));
	CHECK(v.Magnitude() == doctest::Approx(1.0f));
}


TEST_CASE("Normalize leaves the zero vector unchanged")
{
	auto v = Vector2F(0.0f, 0.0f);

	v.Normalize();

	CHECK(v.IsZero());
}


TEST_CASE("Lerp interpolates between endpoints")
{
	auto a = Vector2F(0.0f, 0.0f);
	auto b = Vector2F(10.0f, -20.0f);

	CHECK(Vector2F::Lerp(a, b, 0.0f) == a);
	CHECK(Vector2F::Lerp(a, b, 1.0f) == b);

	auto mid = Vector2F::Lerp(a, b, 0.5f);

	CHECK(mid.x == doctest::Approx(5.0f));
	CHECK(mid.y == doctest::Approx(-10.0f));
}


TEST_CASE("Equal vectors hash equally and work in unordered containers")
{
	auto hasher = std::hash<Vector2F>{};

	CHECK(hasher(Vector2F(1.0f, 2.0f)) == hasher(Vector2F(1.0f, 2.0f)));

	std::unordered_set<Vector2F> set;

	set.insert(Vector2F(1.0f, 2.0f));
	set.insert(Vector2F(1.0f, 2.0f));
	set.insert(Vector2F(2.0f, 1.0f));

	CHECK(set.size() == 2);
}


TEST_CASE("Integer specialization behaves component-wise")
{
	auto a = Vector2I(3, 4);
	auto b = Vector2I(1, 2);

	CHECK(a + b == Vector2I(4, 6));
	CHECK(a - b == Vector2I(2, 2));
	CHECK(a * 2 == Vector2I(6, 8));
	CHECK(Vector2I::Zero.IsZero());
}


TEST_SUITE_END();