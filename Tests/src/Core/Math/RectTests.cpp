#include "Core/Math/Rect.h"
#include <doctest.h>


using namespace DF2D::Core;


TEST_SUITE_BEGIN("Rect");


TEST_CASE("Default rect is zeroed and empty")
{
	RectF rect;

	CHECK(rect.x == 0.0f);
	CHECK(rect.y == 0.0f);
	CHECK(rect.w == 0.0f);
	CHECK(rect.h == 0.0f);
	CHECK(rect.IsEmpty());
}


TEST_CASE("Equality compares all four fields")
{
	auto a = RectF{ .x = 1.0f, .y = 2.0f, .w = 3.0f, .h = 4.0f };
	auto b = RectF{ .x = 1.0f, .y = 2.0f, .w = 3.0f, .h = 4.0f };

	CHECK(a == b);

	b.h = 5.0f;

	CHECK(a != b);
}


TEST_CASE("Contains includes the top-left edge and excludes the bottom-right edge")
{
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK(rect.Contains(0.0f, 0.0f));
	CHECK(rect.Contains(5.0f, 5.0f));
	CHECK(rect.Contains(9.999f, 9.999f));

	CHECK_FALSE(rect.Contains(10.0f, 5.0f));
	CHECK_FALSE(rect.Contains(5.0f, 10.0f));
	CHECK_FALSE(rect.Contains(-0.001f, 5.0f));
	CHECK_FALSE(rect.Contains(5.0f, -0.001f));
}


TEST_CASE("Contains respects the rect origin")
{
	auto rect = RectI{ .x = 10, .y = 20, .w = 5, .h = 5 };

	CHECK(rect.Contains(10, 20));
	CHECK(rect.Contains(14, 24));
	CHECK_FALSE(rect.Contains(9, 20));
	CHECK_FALSE(rect.Contains(15, 20));
	CHECK_FALSE(rect.Contains(10, 25));
}


TEST_CASE("IsEmpty is true for zero or negative extents")
{
	CHECK(RectF{ .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 10.0f }.IsEmpty());
	CHECK(RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 0.0f }.IsEmpty());
	CHECK(RectF{ .x = 0.0f, .y = 0.0f, .w = -1.0f, .h = 10.0f }.IsEmpty());

	CHECK_FALSE(RectF{ .x = 0.0f, .y = 0.0f, .w = 1.0f, .h = 1.0f }.IsEmpty());
}


TEST_SUITE_END();