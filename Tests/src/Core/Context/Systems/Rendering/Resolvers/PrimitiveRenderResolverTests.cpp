#include "Core/Context/Systems/Rendering/Resolvers/Specializations/CircleBatchRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/CircleRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/LineBatchRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/LineRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/PointBatchRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/PointRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/RectBatchRenderResolver.h"
#include "Core/Context/Systems/Rendering/Resolvers/Specializations/RectRenderResolver.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// Every primitive resolver is driven with no camera, so nothing is culled or
	// converted to screen space and the blend mode is the only thing under test.
	ComponentHandle<Camera> NoCamera()
	{
		return ComponentHandle<Camera>();
	}
}


TEST_SUITE_BEGIN("PrimitiveRenderResolvers");


TEST_CASE("Every primitive render data defaults to alpha blending")
{
	CHECK(PointRenderData{}.blendMode == BlendMode::ALPHA);
	CHECK(LineRenderData{}.blendMode == BlendMode::ALPHA);
	CHECK(CircleRenderData{}.blendMode == BlendMode::ALPHA);
	CHECK(RectRenderData{}.blendMode == BlendMode::ALPHA);
}


TEST_CASE("The point resolver forwards its blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	auto point = PointRenderData{ .blendMode = BlendMode::ADDITIVE };

	RenderResolver<PointRenderData>::Render(backend, point, NoCamera(), false);

	CHECK(backend.drawCount == 1);
	CHECK(backend.lastBlendMode == BlendMode::ADDITIVE);
}


TEST_CASE("The line resolver forwards its blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	auto line = LineRenderData{ .blendMode = BlendMode::ADDITIVE };

	RenderResolver<LineRenderData>::Render(backend, line, NoCamera(), false);

	CHECK(backend.lastBlendMode == BlendMode::ADDITIVE);
}


TEST_CASE("The circle resolver forwards its blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	auto circle = CircleRenderData{ .blendMode = BlendMode::NONE };

	RenderResolver<CircleRenderData>::Render(backend, circle, NoCamera(), false);

	CHECK(backend.lastBlendMode == BlendMode::NONE);
}


TEST_CASE("The rect resolver forwards its blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	auto rect = RectRenderData{ .blendMode = BlendMode::ADDITIVE };

	RenderResolver<RectRenderData>::Render(backend, rect, NoCamera(), false);

	CHECK(backend.lastBlendMode == BlendMode::ADDITIVE);
}


TEST_CASE("Batched primitives each carry their own blend mode")
{
	auto backend = MockRenderBackend();

	auto points = PointBatchRenderData{};
	points.pointBatch.push_back(PointRenderData{ .blendMode = BlendMode::ALPHA });
	points.pointBatch.push_back(PointRenderData{ .blendMode = BlendMode::ADDITIVE });
	points.pointBatch.push_back(PointRenderData{ .blendMode = BlendMode::NONE });

	RenderResolver<PointBatchRenderData>::Render(backend, points, NoCamera(), false);

	REQUIRE(backend.blendModes.size() == 3);

	CHECK(backend.blendModes[0] == BlendMode::ALPHA);
	CHECK(backend.blendModes[1] == BlendMode::ADDITIVE);
	CHECK(backend.blendModes[2] == BlendMode::NONE);
}


TEST_CASE("The line batch resolver forwards each line blend mode")
{
	auto backend = MockRenderBackend();

	auto lines = LineBatchRenderData{};
	lines.lineBatch.push_back(LineRenderData{ .blendMode = BlendMode::ADDITIVE });
	lines.lineBatch.push_back(LineRenderData{ .blendMode = BlendMode::ALPHA });

	RenderResolver<LineBatchRenderData>::Render(backend, lines, NoCamera(), false);

	REQUIRE(backend.blendModes.size() == 2);

	CHECK(backend.blendModes[0] == BlendMode::ADDITIVE);
	CHECK(backend.blendModes[1] == BlendMode::ALPHA);
}


TEST_CASE("The circle batch resolver forwards each circle blend mode")
{
	auto backend = MockRenderBackend();

	auto circles = CircleBatchRenderData{};
	circles.circleBatch.push_back(CircleRenderData{ .blendMode = BlendMode::ADDITIVE });
	circles.circleBatch.push_back(CircleRenderData{ .blendMode = BlendMode::NONE });

	RenderResolver<CircleBatchRenderData>::Render(backend, circles, NoCamera(), false);

	REQUIRE(backend.blendModes.size() == 2);

	CHECK(backend.blendModes[0] == BlendMode::ADDITIVE);
	CHECK(backend.blendModes[1] == BlendMode::NONE);
}


TEST_CASE("The rect batch resolver forwards each rect blend mode")
{
	auto backend = MockRenderBackend();

	auto rects = RectBatchRenderData{};
	rects.rectBatch.push_back(RectRenderData{ .blendMode = BlendMode::NONE });
	rects.rectBatch.push_back(RectRenderData{ .blendMode = BlendMode::ADDITIVE });

	RenderResolver<RectBatchRenderData>::Render(backend, rects, NoCamera(), false);

	REQUIRE(backend.blendModes.size() == 2);

	CHECK(backend.blendModes[0] == BlendMode::NONE);
	CHECK(backend.blendModes[1] == BlendMode::ADDITIVE);
}


TEST_CASE("An empty primitive batch issues no draw calls")
{
	auto backend = MockRenderBackend();

	RenderResolver<PointBatchRenderData>::Render(backend, PointBatchRenderData{}, NoCamera(), false);
	RenderResolver<LineBatchRenderData>::Render(backend, LineBatchRenderData{}, NoCamera(), false);
	RenderResolver<CircleBatchRenderData>::Render(backend, CircleBatchRenderData{}, NoCamera(), false);
	RenderResolver<RectBatchRenderData>::Render(backend, RectBatchRenderData{}, NoCamera(), false);

	CHECK(backend.drawCount == 0);
}


TEST_SUITE_END();