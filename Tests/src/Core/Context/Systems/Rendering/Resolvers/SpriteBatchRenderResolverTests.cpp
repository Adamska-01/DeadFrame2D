#include "Core/Context/Systems/Rendering/Resolvers/Specializations/SpriteBatchRenderResolver.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	SpriteRenderData MakeSprite(BlendMode blendMode)
	{
		return SpriteRenderData
		{
			.texture = 1,
			.destRect = RectF{ .x = 0.0f, .y = 0.0f, .w = 8.0f, .h = 8.0f },
			.blendMode = blendMode
		};
	}
}


TEST_SUITE_BEGIN("SpriteBatchRenderResolver");


TEST_CASE("Render issues one backend draw call per sprite in the batch")
{
	auto backend = MockRenderBackend();

	auto batch = SpriteBatchRenderData{};
	batch.spriteBatch.push_back(MakeSprite(BlendMode::ALPHA));
	batch.spriteBatch.push_back(MakeSprite(BlendMode::ALPHA));
	batch.spriteBatch.push_back(MakeSprite(BlendMode::ALPHA));

	RenderResolver<SpriteBatchRenderData>::Render(backend, batch, ComponentHandle<Camera>(), false);

	CHECK(backend.drawCount == 3);
}


TEST_CASE("Render forwards each sprite own blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	auto batch = SpriteBatchRenderData{};
	batch.spriteBatch.push_back(MakeSprite(BlendMode::ALPHA));
	batch.spriteBatch.push_back(MakeSprite(BlendMode::ADDITIVE));
	batch.spriteBatch.push_back(MakeSprite(BlendMode::NONE));

	RenderResolver<SpriteBatchRenderData>::Render(backend, batch, ComponentHandle<Camera>(), false);

	REQUIRE(backend.blendModes.size() == 3);

	CHECK(backend.blendModes[0] == BlendMode::ALPHA);
	CHECK(backend.blendModes[1] == BlendMode::ADDITIVE);
	CHECK(backend.blendModes[2] == BlendMode::NONE);
}


TEST_CASE("Render draws nothing for an empty batch")
{
	auto backend = MockRenderBackend();

	auto batch = SpriteBatchRenderData{};

	RenderResolver<SpriteBatchRenderData>::Render(backend, batch, ComponentHandle<Camera>(), false);

	CHECK(backend.drawCount == 0);
}


TEST_SUITE_END();