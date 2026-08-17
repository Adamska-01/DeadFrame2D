#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Models/Rendering/TileEditors/Tiled/TiledMap.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;
using namespace DF2D::Models;


namespace
{
	// 2x2 map, one 2x2 tileset (ids 1-4), one layer: {{1,2},{0,3}} (0 = empty tile).
	std::shared_ptr<TiledMap> MakeTiledMap(TextureManager* tm)
	{
		auto map = std::make_shared<TiledMap>();
		map->width = 2;
		map->height = 2;
		map->tileSize = 16;

		map->tileSets.emplace_back(
			/*firstID*/ 1, /*lastID*/ 4, /*rowCount*/ 2, /*columnCount*/ 2,
			/*tileCount*/ 4, /*tileSize*/ 16, /*source*/ "tiles.png", tm);

		auto layer = TiledLayer{};
		layer.name = "Layer1";
		layer.Data = { { 1, 2 }, { 0, 3 } };
		map->layers.push_back(std::move(layer));

		return map;
	}


	struct TiledMapRendererFixture
	{
		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* textureBackend;

		std::unique_ptr<Renderer> renderer;

		MockRenderBackend* renderBackend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;

		std::shared_ptr<TiledMap> tileMap;


		TiledMapRendererFixture()
		{
			auto tb = std::make_unique<MockTextureBackend>();
			textureBackend = tb.get();
			textureManager = std::make_unique<TextureManager>(std::move(tb));

			auto rb = std::make_unique<MockRenderBackend>();
			renderBackend = rb.get();
			renderer = std::make_unique<Renderer>(std::move(rb));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get(), .renderer = renderer.get() });

			owner = scene->Create<TestGameObject>();

			tileMap = MakeTiledMap(textureManager.get());
		}

		ComponentHandle<TiledMapCompatibleRenderer> MakeRenderer(bool extendMapToRenderTarget)
		{
			auto mapRenderer = owner->AddComponent<TiledMapCompatibleRenderer>(tileMap, extendMapToRenderTarget);
			mapRenderer->Init();

			return mapRenderer;
		}
	};
}


TEST_SUITE_BEGIN("TiledMapCompatibleRenderer");


TEST_CASE("GetTileMap returns the map passed to the constructor")
{
	TiledMapRendererFixture fixture;

	auto renderer = fixture.MakeRenderer(/*extendMapToRenderTarget*/ false);

	CHECK(renderer->GetTileMap() == fixture.tileMap);
}

TEST_CASE("Init without extendMapToRenderTarget leaves the renderer's resolution target untouched")
{
	TiledMapRendererFixture fixture;

	fixture.MakeRenderer(/*extendMapToRenderTarget*/ false);

	CHECK(fixture.renderBackend->resolutionTarget == Vector2I{ 1920, 1080 });
}

TEST_CASE("Init with extendMapToRenderTarget resizes the resolution target to width*tileSize x height*tileSize")
{
	TiledMapRendererFixture fixture;

	fixture.MakeRenderer(/*extendMapToRenderTarget*/ true);

	// 2 columns/rows * 16px tileSize == 32x32; must stay symmetric (no leftover
	// asymmetric padding on just one axis).
	CHECK(fixture.renderBackend->resolutionTarget == Vector2I{ 32, 32 });
}

TEST_CASE("Draw does not throw for a fully-mapped layer")
{
	TiledMapRendererFixture fixture;

	auto renderer = fixture.MakeRenderer(/*extendMapToRenderTarget*/ false);

	CHECK_NOTHROW(renderer->Draw());
}

TEST_CASE("Draw does not throw when a layer references a tile ID outside every tileset's range")
{
	// Characterization test: TiledMapCompatibleRenderer::Draw looks up
	// tileIDToTileSet[tileID] with operator[], so an unmapped ID silently falls
	// back to tileSetIndex 0 instead of erroring. Documented as a known gap;
	// this only pins down that it doesn't crash.
	TiledMapRendererFixture fixture;

	fixture.tileMap->layers[0].Data[0][0] = 999;

	auto renderer = fixture.MakeRenderer(/*extendMapToRenderTarget*/ false);

	CHECK_NOTHROW(renderer->Draw());
}


TEST_SUITE_END();