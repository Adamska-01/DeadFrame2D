#include "Data/Components/UI/Layout/UIAnchor.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;


TEST_SUITE_BEGIN("UIAnchor");


TEST_CASE("GetAnchorFromPreset returns the expected pivot for every preset")
{
	CHECK(GetAnchorFromPreset(UIAnchor::TOP_LEFT) == Vector2F(0.0f, 0.0f));
	CHECK(GetAnchorFromPreset(UIAnchor::TOP_CENTER) == Vector2F(0.5f, 0.0f));
	CHECK(GetAnchorFromPreset(UIAnchor::TOP_RIGHT) == Vector2F(1.0f, 0.0f));
	CHECK(GetAnchorFromPreset(UIAnchor::CENTER_LEFT) == Vector2F(0.0f, 0.5f));
	CHECK(GetAnchorFromPreset(UIAnchor::CENTER) == Vector2F(0.5f, 0.5f));
	CHECK(GetAnchorFromPreset(UIAnchor::CENTER_RIGHT) == Vector2F(1.0f, 0.5f));
	CHECK(GetAnchorFromPreset(UIAnchor::BOTTOM_LEFT) == Vector2F(0.0f, 1.0f));
	CHECK(GetAnchorFromPreset(UIAnchor::BOTTOM_CENTER) == Vector2F(0.5f, 1.0f));
	CHECK(GetAnchorFromPreset(UIAnchor::BOTTOM_RIGHT) == Vector2F(1.0f, 1.0f));
}

TEST_CASE("GetAnchorFromPreset falls back to CENTER for an out-of-range value")
{
	CHECK(GetAnchorFromPreset(static_cast<UIAnchor>(999)) == Vector2F(0.5f, 0.5f));
}


TEST_SUITE_END();