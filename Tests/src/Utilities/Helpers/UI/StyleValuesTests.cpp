#include "Utilities/Helpers/UI/StyleValues.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("StyleValues");


TEST_CASE("ToPixels appends px and trims trailing zeros")
{
	CHECK(StyleValues::ToPixels(12.0f) == "12px");
	CHECK(StyleValues::ToPixels(12.5f) == "12.5px");
}

TEST_CASE("ToDp appends dp and trims trailing zeros")
{
	CHECK(StyleValues::ToDp(18.0f) == "18dp");
	CHECK(StyleValues::ToDp(18.5f) == "18.5dp");
}

TEST_CASE("ToPixels and ToDp format the same number, differing only in unit")
{
	// The two units compute identically at a 1:1 density ratio and only diverge once a canvas scales
	// -- px stays a true device pixel, dp scales with it. Callers pick the unit, not the formatting.
	CHECK(StyleValues::ToPixels(24.0f) == "24px");
	CHECK(StyleValues::ToDp(24.0f) == "24dp");
}

TEST_CASE("ToNumber trims trailing zeros and carries no unit")
{
	CHECK(StyleValues::ToNumber(1.0f) == "1");
	CHECK(StyleValues::ToNumber(0.5f) == "0.5");
}

TEST_CASE("ToPercent scales a 0-1 ratio to a percentage")
{
	CHECK(StyleValues::ToPercent(0.0f) == "0.000000%");
	CHECK(StyleValues::ToPercent(0.5f) == "50.000000%");
	CHECK(StyleValues::ToPercent(1.0f) == "100.000000%");
}

TEST_CASE("ToColor formats each channel as two lowercase hex digits")
{
	CHECK(StyleValues::ToColor(Color{ 0, 0, 0, 0 }) == "#00000000");
	CHECK(StyleValues::ToColor(Color{ 255, 255, 255, 255 }) == "#ffffffff");
	CHECK(StyleValues::ToColor(Color{ 0x1c, 0x1c, 0x20, 0xff }) == "#1c1c20ff");
}


TEST_SUITE_END();