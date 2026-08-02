#include "Core/Math/Color.h"
#include <doctest.h>


using namespace DF2D::Core;


TEST_SUITE_BEGIN("Color");


TEST_CASE("Default color is opaque black")
{
	Color color;

	CHECK(color.r == 0);
	CHECK(color.g == 0);
	CHECK(color.b == 0);
	CHECK(color.a == 255);
}


TEST_CASE("Equality compares all four channels")
{
	auto a = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };
	auto b = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };

	CHECK(a == b);
}


TEST_CASE("Inequality triggers on any differing channel")
{
	auto base = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };

	CHECK(base != Color{ .r = 11, .g = 20, .b = 30, .a = 40 });
	CHECK(base != Color{ .r = 10, .g = 21, .b = 30, .a = 40 });
	CHECK(base != Color{ .r = 10, .g = 20, .b = 31, .a = 40 });
	CHECK(base != Color{ .r = 10, .g = 20, .b = 30, .a = 41 });
}


TEST_SUITE_END();