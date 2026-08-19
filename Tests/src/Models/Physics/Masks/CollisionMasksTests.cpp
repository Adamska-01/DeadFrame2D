#include <doctest.h>
#include "Models/Physics/Masks/CollisionMasks.h"


using namespace DF2D::Models;


TEST_SUITE_BEGIN("CollisionMasks");


TEST_CASE("GetMaskFlagByName returns value for known category")
{
	CollisionMasks masks;
	masks.categories.push_back(NamedBitflag{ "PLAYER", 0x0001 });
	masks.categories.push_back(NamedBitflag{ "ENEMY", 0x0002 });

	CHECK(masks.GetMaskFlagByName("ENEMY") == 0x0002);
}


TEST_CASE("GetMaskFlagByName returns 0 for unknown category")
{
	CollisionMasks masks;
	masks.categories.push_back(NamedBitflag{ "PLAYER", 0x0001 });

	CHECK(masks.GetMaskFlagByName("GROUND") == 0);
}


TEST_CASE("GetMaskFlagByName returns 0 when categories is empty")
{
	CollisionMasks masks;

	CHECK(masks.GetMaskFlagByName("ANYTHING") == 0);
}


TEST_SUITE_END();