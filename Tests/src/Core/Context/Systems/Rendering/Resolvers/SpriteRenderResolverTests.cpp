#include "Core/Context/Systems/Rendering/Resolvers/Specializations/SpriteRenderResolver.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteRenderData.h"
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


TEST_SUITE_BEGIN("SpriteRenderResolver");


TEST_CASE("SpriteRenderData defaults to alpha blending")
{
	auto sprite = SpriteRenderData{};

	CHECK(sprite.blendMode == BlendMode::ALPHA);
}


TEST_CASE("Render forwards the sprite blend mode to the render backend")
{
	auto backend = MockRenderBackend();

	RenderResolver<SpriteRenderData>::Render(backend, MakeSprite(BlendMode::ALPHA), ComponentHandle<Camera>(), false);

	CHECK(backend.drawCount == 1);
	CHECK(backend.lastBlendMode == BlendMode::ALPHA);
}


TEST_CASE("Render forwards additive blending unchanged")
{
	auto backend = MockRenderBackend();

	RenderResolver<SpriteRenderData>::Render(backend, MakeSprite(BlendMode::ADDITIVE), ComponentHandle<Camera>(), false);

	CHECK(backend.lastBlendMode == BlendMode::ADDITIVE);
}


TEST_SUITE_END();