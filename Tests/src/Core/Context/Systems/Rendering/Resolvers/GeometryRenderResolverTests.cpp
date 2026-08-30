#include "Core/Context/Systems/Rendering/Resolvers/Specializations/GeometryRenderResolver.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/GeometryRenderData.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Appends a quad (4 vertices, 6 indices) and returns a command addressing just that quad. */
	GeometryCommand AppendQuad(GeometryDrawList& drawList, TextureID texture)
	{
		auto command = GeometryCommand
		{
			.texture = texture,
			.vertexOffset = static_cast<uint32_t>(drawList.vertices.size()),
			.vertexCount = 4,
			.indexOffset = static_cast<uint32_t>(drawList.indices.size()),
			.indexCount = 6
		};

		drawList.vertices.insert(drawList.vertices.end(), 4, Vertex2D{});
		drawList.indices.insert(drawList.indices.end(), { 0u, 1u, 2u, 2u, 3u, 0u });

		return command;
	}

	std::shared_ptr<const GeometryDrawList> Share(GeometryDrawList drawList)
	{
		return std::make_shared<const GeometryDrawList>(std::move(drawList));
	}

	void Render(MockRenderBackend& backend, std::shared_ptr<const GeometryDrawList> drawList)
	{
		RenderResolver<GeometryRenderData>::Render(
			backend,
			GeometryRenderData{ .drawList = std::move(drawList) },
			ComponentHandle<Camera>(),
			false);
	}
}


TEST_SUITE_BEGIN("GeometryRenderResolver");


TEST_CASE("Render tolerates a null draw list")
{
	auto backend = MockRenderBackend();

	Render(backend, nullptr);

	CHECK(backend.drawGeometryCount == 0);
}


TEST_CASE("Render replays every command in submission order")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	drawList.commands.push_back(AppendQuad(drawList, 7));
	drawList.commands.push_back(AppendQuad(drawList, 9));

	Render(backend, Share(std::move(drawList)));

	CHECK(backend.drawGeometryCount == 2);
	CHECK(backend.geometryTextures[0] == 7);
	CHECK(backend.geometryTextures[1] == 9);
}


TEST_CASE("Render hands each command only its own slice of the shared pools")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	drawList.commands.push_back(AppendQuad(drawList, 1));
	drawList.commands.push_back(AppendQuad(drawList, 2));

	// Both quads live in one pool, so a resolver that forwarded the whole buffer would report 8 and 12.
	Render(backend, Share(std::move(drawList)));

	CHECK(backend.geometryVertexCounts[0] == 4);
	CHECK(backend.geometryIndexCounts[0] == 6);
	CHECK(backend.geometryVertexCounts[1] == 4);
	CHECK(backend.geometryIndexCounts[1] == 6);
}


TEST_CASE("Render forwards the per-command translation and blend mode")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto command = AppendQuad(drawList, 1);
	command.translation = Vector2F(12.0f, -5.0f);
	command.blendMode = BlendMode::PREMULTIPLIED_ALPHA;

	drawList.commands.push_back(command);

	Render(backend, Share(std::move(drawList)));

	CHECK(backend.geometryTranslations[0] == Vector2F(12.0f, -5.0f));
	CHECK(backend.lastBlendMode == BlendMode::PREMULTIPLIED_ALPHA);
}


TEST_CASE("Render applies each command's scissor rectangle before drawing it")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto clipped = AppendQuad(drawList, 1);
	clipped.clipRect = RectI{ .x = 4, .y = 8, .w = 16, .h = 32 };

	drawList.commands.push_back(clipped);

	Render(backend, Share(std::move(drawList)));

	REQUIRE(backend.clipRects.size() >= 1);
	REQUIRE(backend.clipRects[0].has_value());
	CHECK(*backend.clipRects[0] == RectI{ .x = 4, .y = 8, .w = 16, .h = 32 });
}


TEST_CASE("Render clears the scissor rectangle when the draw list ends")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto clipped = AppendQuad(drawList, 1);
	clipped.clipRect = RectI{ .x = 0, .y = 0, .w = 8, .h = 8 };

	drawList.commands.push_back(clipped);

	// Clipping is renderer state: leaving it applied would silently clip the next pass.
	Render(backend, Share(std::move(drawList)));

	CHECK_FALSE(backend.lastClipRect.has_value());
}


TEST_CASE("Render skips commands that reach past the vertex pool")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto command = AppendQuad(drawList, 1);
	command.vertexCount = 99;

	drawList.commands.push_back(command);

	Render(backend, Share(std::move(drawList)));

	CHECK(backend.drawGeometryCount == 0);
}


TEST_CASE("Render skips commands that reach past the index pool")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto command = AppendQuad(drawList, 1);
	command.indexCount = 99;

	drawList.commands.push_back(command);

	Render(backend, Share(std::move(drawList)));

	CHECK(backend.drawGeometryCount == 0);
}


TEST_CASE("Render skips empty commands")
{
	auto backend = MockRenderBackend();
	auto drawList = GeometryDrawList();

	auto command = AppendQuad(drawList, 1);
	command.indexCount = 0;

	drawList.commands.push_back(command);

	Render(backend, Share(std::move(drawList)));

	CHECK(backend.drawGeometryCount == 0);
}


TEST_SUITE_END();