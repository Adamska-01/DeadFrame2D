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


TEST_CASE("Color Lerp returns the start colour at t equal to zero")
{
	auto from = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };
	auto to = Color{ .r = 200, .g = 210, .b = 220, .a = 230 };

	CHECK(Color::Lerp(from, to, 0.0f) == from);
}


TEST_CASE("Color Lerp returns the end colour at t equal to one")
{
	auto from = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };
	auto to = Color{ .r = 200, .g = 210, .b = 220, .a = 230 };

	CHECK(Color::Lerp(from, to, 1.0f) == to);
}


TEST_CASE("Color Lerp interpolates every channel including alpha")
{
	auto from = Color{ .r = 0, .g = 0, .b = 0, .a = 0 };
	auto to = Color{ .r = 100, .g = 200, .b = 50, .a = 255 };

	auto result = Color::Lerp(from, to, 0.5f);

	CHECK(result.r == 50);
	CHECK(result.g == 100);
	CHECK(result.b == 25);
	CHECK(result.a == 128);
}


TEST_CASE("Color Lerp clamps t outside the zero to one range")
{
	auto from = Color{ .r = 10, .g = 20, .b = 30, .a = 40 };
	auto to = Color{ .r = 200, .g = 210, .b = 220, .a = 230 };

	CHECK(Color::Lerp(from, to, -3.0f) == from);
	CHECK(Color::Lerp(from, to, 4.0f) == to);
}


TEST_CASE("Color Lerp between black and white does not overflow the byte channels")
{
	auto black = Color{ .r = 0, .g = 0, .b = 0, .a = 255 };
	auto white = Color{ .r = 255, .g = 255, .b = 255, .a = 255 };

	auto staysWithinRange = true;

	for (auto step = 0; step <= 100; ++step)
	{
		auto result = Color::Lerp(black, white, static_cast<float>(step) / 100.0f);

		if (result.a != 255 || result.r != result.g || result.g != result.b)
		{
			staysWithinRange = false;
		}
	}

	CHECK(staysWithinRange);
}


TEST_SUITE_END();