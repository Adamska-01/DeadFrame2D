#include "Utilities/Collisions/CollisionUtils.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Utilities::Collision;


TEST_SUITE_BEGIN("CollisionUtils");


TEST_CASE("PointVsBox includes the top-left edge and excludes the bottom-right edge")
{
	auto box = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK(PointVsBox(Vector2F(0.0f, 0.0f), box));
	CHECK(PointVsBox(Vector2F(5.0f, 5.0f), box));

	CHECK_FALSE(PointVsBox(Vector2F(10.0f, 5.0f), box));
	CHECK_FALSE(PointVsBox(Vector2F(5.0f, 10.0f), box));
	CHECK_FALSE(PointVsBox(Vector2F(-0.001f, 5.0f), box));
}


TEST_CASE("PointVsCircle is inclusive of the radius")
{
	auto circle = Circle(Vector2F(0.0f, 0.0f), 5.0f);

	CHECK(PointVsCircle(Vector2F(0.0f, 0.0f), circle));
	CHECK(PointVsCircle(Vector2F(5.0f, 0.0f), circle));
	CHECK(PointVsCircle(Vector2F(3.0f, 4.0f), circle));

	CHECK_FALSE(PointVsCircle(Vector2F(5.001f, 0.0f), circle));
}


TEST_CASE("RectVsRect (float) detects overlap and rejects merely-touching rects")
{
	auto a = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };
	auto overlapping = RectF{ .x = 5.0f, .y = 5.0f, .w = 10.0f, .h = 10.0f };
	auto disjoint = RectF{ .x = 100.0f, .y = 100.0f, .w = 10.0f, .h = 10.0f };
	auto touchingRightEdge = RectF{ .x = 10.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };
	auto contained = RectF{ .x = 2.0f, .y = 2.0f, .w = 2.0f, .h = 2.0f };

	CHECK(RectVsRect(a, overlapping));
	CHECK_FALSE(RectVsRect(a, disjoint));
	CHECK_FALSE(RectVsRect(a, touchingRightEdge));
	CHECK(RectVsRect(a, contained));
}


TEST_CASE("RectVsRect (int) detects overlap and rejects merely-touching rects")
{
	auto a = RectI{ .x = 0, .y = 0, .w = 10, .h = 10 };
	auto overlapping = RectI{ .x = 5, .y = 5, .w = 10, .h = 10 };
	auto disjoint = RectI{ .x = 100, .y = 100, .w = 10, .h = 10 };
	auto touchingRightEdge = RectI{ .x = 10, .y = 0, .w = 10, .h = 10 };

	CHECK(RectVsRect(a, overlapping));
	CHECK_FALSE(RectVsRect(a, disjoint));
	CHECK_FALSE(RectVsRect(a, touchingRightEdge));
}


TEST_CASE("RectVsRect (float) with a zero-width rect never reports overlap")
{
	auto zeroWidth = RectF{ .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 10.0f };
	auto other = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK_FALSE(RectVsRect(zeroWidth, other));
}


TEST_CASE("CircleVsRect clamps to the nearest point and is inclusive of the radius")
{
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK(CircleVsRect(Circle(Vector2F(5.0f, 5.0f), 1.0f), rect));
	CHECK(CircleVsRect(Circle(Vector2F(15.0f, 5.0f), 5.0f), rect));
	CHECK_FALSE(CircleVsRect(Circle(Vector2F(20.0f, 5.0f), 5.0f), rect));

	// Tangent: closest point is exactly `radius` away.
	CHECK(CircleVsRect(Circle(Vector2F(15.0f, 5.0f), 5.001f), rect));
}


TEST_CASE("SegmentVsRect hits a diagonal segment crossing the rect")
{
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK(SegmentVsRect(Vector2F(-5.0f, 5.0f), Vector2F(15.0f, 5.0f), rect));
}


TEST_CASE("SegmentVsRect misses a segment whose bounding box does not reach the rect")
{
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK_FALSE(SegmentVsRect(Vector2F(-5.0f, -5.0f), Vector2F(-1.0f, -1.0f), rect));
}


TEST_CASE("SegmentVsRect with an exactly-vertical direction hits when the segment crosses the slab")
{
	// dx == 0.0f exactly: exercises the `p[i] == 0.0f` branch in the x axis.
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK(SegmentVsRect(Vector2F(5.0f, -5.0f), Vector2F(5.0f, 15.0f), rect));
}


TEST_CASE("SegmentVsRect with an exactly-vertical direction on the rect's edge is a miss")
{
	// dx == 0.0f exactly, and the segment's x sits exactly on the rect's right edge:
	// the `q[i] < EPSILON` check inside the p[i]==0.0f branch rejects it, so an
	// edge-touching vertical segment is treated as a miss (consistent with RectVsRect's
	// strict-overlap-only semantics).
	auto rect = RectF{ .x = 0.0f, .y = 0.0f, .w = 10.0f, .h = 10.0f };

	CHECK_FALSE(SegmentVsRect(Vector2F(10.0f, -5.0f), Vector2F(10.0f, 15.0f), rect));
}


TEST_CASE("RayVsRect hits a horizontal ray and reports the entry point and normal")
{
	auto target = RectI{ .x = 0, .y = 0, .w = 10, .h = 10 };
	auto contactPoint = Vector2F::Zero;
	auto contactNormal = Vector2F::Zero;
	auto tHitNear = 0.0f;

	auto hit = RayVsRect(Vector2F(-5.0f, 5.0f), Vector2F::Right, target, contactPoint, contactNormal, tHitNear);

	REQUIRE(hit);
	CHECK(contactPoint.x == doctest::Approx(0.0f));
	CHECK(contactPoint.y == doctest::Approx(5.0f));
	CHECK(contactNormal == Vector2F::Left);
	CHECK(tHitNear == doctest::Approx(5.0f));
}


TEST_CASE("RayVsRect hits a vertical ray and reports the entry point and normal")
{
	auto target = RectI{ .x = 0, .y = 0, .w = 10, .h = 10 };
	auto contactPoint = Vector2F::Zero;
	auto contactNormal = Vector2F::Zero;
	auto tHitNear = 0.0f;

	auto hit = RayVsRect(Vector2F(5.0f, -5.0f), Vector2F::Down, target, contactPoint, contactNormal, tHitNear);

	REQUIRE(hit);
	CHECK(contactPoint.x == doctest::Approx(5.0f));
	CHECK(contactPoint.y == doctest::Approx(0.0f));
	CHECK(contactNormal == Vector2F::Up);
	CHECK(tHitNear == doctest::Approx(5.0f));
}


TEST_CASE("RayVsRect misses a ray pointing away from the target")
{
	auto target = RectI{ .x = 0, .y = 0, .w = 10, .h = 10 };
	auto contactPoint = Vector2F::Zero;
	auto contactNormal = Vector2F::Zero;
	auto tHitNear = 0.0f;

	auto hit = RayVsRect(Vector2F(-5.0f, -5.0f), Vector2F(-1.0f, -1.0f), target, contactPoint, contactNormal, tHitNear);

	CHECK_FALSE(hit);
}


TEST_CASE("RayVsRect misses when the ray is parallel to and coincident with an edge")
{
	// ray_dir.y == 0 and the numerator (target.y - origin.y) is also 0, producing
	// 0 * inf == NaN in t_near.y. The existing std::isnan guard treats this as a miss,
	// even though the ray runs exactly along the target's top edge extended outward.
	// This pins that current (documented, not "fixed") limitation of the slab test.
	auto target = RectI{ .x = 0, .y = 0, .w = 10, .h = 10 };
	auto contactPoint = Vector2F::Zero;
	auto contactNormal = Vector2F::Zero;
	auto tHitNear = 0.0f;

	auto hit = RayVsRect(Vector2F(-5.0f, 0.0f), Vector2F::Right, target, contactPoint, contactNormal, tHitNear);

	CHECK_FALSE(hit);
}


TEST_SUITE_END();
