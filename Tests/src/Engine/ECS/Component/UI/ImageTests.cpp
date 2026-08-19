#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct ImageFixture
	{
		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* backend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> canvasOwner;

		ObjectHandle<TestGameObject> owner;


		ImageFixture()
		{
			auto mock = std::make_unique<MockTextureBackend>();
			backend = mock.get();
			textureManager = std::make_unique<TextureManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

			canvasOwner = scene->Create<TestGameObject>();
			canvasOwner->AddComponent<Canvas>();

			owner = scene->Create<TestGameObject>();
			owner->SetParent(canvasOwner);
		}
	};
}


TEST_SUITE_BEGIN("Image");


TEST_CASE("Init resolves the TextureManager from CoreContext")
{
	ImageFixture fixture;

	auto image = fixture.owner->AddComponent<Image>();

	CHECK_NOTHROW(image->Init());
}

TEST_CASE("LoadSprite before Init does not crash and leaves the widget size untouched")
{
	ImageFixture fixture;

	auto image = fixture.owner->AddComponent<Image>();

	// textureManager is only wired during Init(); mirrors blueprint constructors
	// that may touch an Image before Scene ever calls Init() on it (see SpriteRenderer).
	CHECK_NOTHROW(image->LoadSprite("early.png"));

	CHECK(fixture.backend->loadCount == 0);
	CHECK(image->GetWidgetSize().x == doctest::Approx(1.0f));
	CHECK(image->GetWidgetSize().y == doctest::Approx(1.0f));
}

TEST_CASE("LoadSprite after Init loads the texture and resizes the widget to match")
{
	ImageFixture fixture;
	fixture.backend->nextSize = { 64, 32 };

	auto image = fixture.owner->AddComponent<Image>();
	image->Init();

	image->LoadSprite("player.png");

	CHECK(fixture.backend->loadCount == 1);
	CHECK(fixture.backend->lastLoadedFile == "player.png");
	CHECK(image->GetWidgetSize().x == doctest::Approx(64.0f));
	CHECK(image->GetWidgetSize().y == doctest::Approx(32.0f));
}

TEST_CASE("SetColor does not throw")
{
	ImageFixture fixture;

	auto image = fixture.owner->AddComponent<Image>();
	image->Init();

	CHECK_NOTHROW(image->SetColor(10, 20, 30, 40));
}

TEST_CASE("Draw does not throw with no loaded sprite (falls back to a filled rect)")
{
	ImageFixture fixture;

	auto image = fixture.owner->AddComponent<Image>();
	image->Init();
	image->Update(0.016f);

	CHECK_NOTHROW(image->Draw());
}

TEST_CASE("Draw does not throw once a sprite is loaded")
{
	ImageFixture fixture;

	auto image = fixture.owner->AddComponent<Image>();
	image->Init();
	image->LoadSprite("player.png");
	image->Update(0.016f);

	CHECK_NOTHROW(image->Draw());
}

TEST_CASE("Update throws when there is no Canvas ancestor")
{
	ImageFixture fixture;

	auto orphan = fixture.scene->Create<TestGameObject>();
	auto image = orphan->AddComponent<Image>();
	image->Init();

	CHECK_THROWS_AS(image->Update(0.016f), std::invalid_argument);
}


TEST_SUITE_END();
