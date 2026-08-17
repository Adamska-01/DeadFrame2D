#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Engine/ECS/Component/Rendering/SpriteRenderer.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct SpriteRendererFixture
	{
		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* backend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;


		explicit SpriteRendererFixture(std::string_view texturePath = "")
		{
			auto mock = std::make_unique<MockTextureBackend>();
			backend = mock.get();
			textureManager = std::make_unique<TextureManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

			owner = scene->Create<TestGameObject>();
		}
	};
}


TEST_SUITE_BEGIN("SpriteRenderer");


TEST_CASE("Init with an empty texturePath does not load a texture")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("");
	spriteRenderer->Init();

	CHECK(spriteRenderer->GetTexture() == 0);
	CHECK(fixture.backend->loadCount == 0);
}

TEST_CASE("Init with a non-empty texturePath loads the texture via TextureManager")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("player.png");
	spriteRenderer->Init();

	CHECK(spriteRenderer->GetTexture() != 0);
	CHECK(fixture.backend->lastLoadedFile == "player.png");
}

TEST_CASE("LoadSprite called before Init does not crash and leaves the texture unset")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("");

	// textureManager is only wired during Init(); mirrors blueprint constructors
	// that may touch a SpriteRenderer before Scene ever calls Init() on it.
	CHECK_NOTHROW(spriteRenderer->LoadSprite("early.png"));

	CHECK(spriteRenderer->GetTexture() == 0);
	CHECK(fixture.backend->loadCount == 0);
}

TEST_CASE("LoadSprite after Init loads a new texture and replaces the old one")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("idle.png");
	spriteRenderer->Init();

	auto firstTexture = spriteRenderer->GetTexture();

	spriteRenderer->LoadSprite("run.png");

	CHECK(fixture.backend->lastLoadedFile == "run.png");
	CHECK(spriteRenderer->GetTexture() != firstTexture);
}

TEST_CASE("GetSortOrder defaults to DefaultSortOrders::SPRITE_RENDERER")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("");

	CHECK(spriteRenderer->GetSortOrder() == DefaultSortOrders::SPRITE_RENDERER);
}

TEST_CASE("SetSortOrder/GetSortOrder round-trip")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("");
	spriteRenderer->SetSortOrder(42);

	CHECK(spriteRenderer->GetSortOrder() == 42);
}

TEST_CASE("Draw does not throw with no camera in the scene")
{
	SpriteRendererFixture fixture;

	auto spriteRenderer = fixture.owner->AddComponent<SpriteRenderer>("player.png");
	spriteRenderer->Init();

	CHECK_NOTHROW(spriteRenderer->Draw());
}


TEST_SUITE_END();