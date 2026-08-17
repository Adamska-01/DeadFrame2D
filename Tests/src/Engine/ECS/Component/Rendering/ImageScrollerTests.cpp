#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Data/Components/Rendering/ScrollDirection.h"
#include "Engine/ECS/Component/Rendering/ImageScroller.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct ImageScrollerFixture
	{
		EventDispatcher eventDispatcher;

		std::unique_ptr<Renderer> renderer;

		MockRenderBackend* renderBackend;

		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* textureBackend;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> owner;


		ImageScrollerFixture()
		{
			auto rb = std::make_unique<MockRenderBackend>();
			renderBackend = rb.get();
			renderer = std::make_unique<Renderer>(std::move(rb));

			auto tb = std::make_unique<MockTextureBackend>();
			textureBackend = tb.get();
			textureManager = std::make_unique<TextureManager>(std::move(tb));

			scene = std::make_shared<TestScene>(&eventDispatcher);
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get(), .renderer = renderer.get() });

			owner = scene->Spawn<TestGameObject>();
		}

		// spriteSize comes from the mock texture backend; tileSize defaults to 64x64.
		ComponentHandle<ImageScroller> MakeScroller(
			ScrollDirection direction = ScrollDirection::HORIZONTAL,
			float scrollSpeed = 10.0f,
			std::string_view source = "bg.png")
		{
			auto scroller = owner->AddComponent<ImageScroller>(source, direction, scrollSpeed);
			scroller->Init();

			return scroller;
		}
	};
}


TEST_SUITE_BEGIN("ImageScroller");


TEST_CASE("Init throws when CoreContext/ServiceContext has no EventDispatcher")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto scroller = owner->AddComponent<ImageScroller>("bg.png", ScrollDirection::HORIZONTAL, 10.0f);

	CHECK_THROWS_AS(scroller->Init(), std::invalid_argument);
}

TEST_CASE("Draw does not throw when no sprite has been loaded")
{
	ImageScrollerFixture fixture;

	auto scroller = fixture.owner->AddComponent<ImageScroller>("", ScrollDirection::HORIZONTAL, 10.0f);
	scroller->Init();

	CHECK_NOTHROW(scroller->Draw());
}

TEST_CASE("Draw does not throw once a sprite is loaded")
{
	ImageScrollerFixture fixture;

	auto scroller = fixture.MakeScroller();

	CHECK_NOTHROW(scroller->Draw());
}

TEST_CASE("A render-target-size-changed event does not crash a subsequent Draw")
{
	ImageScrollerFixture fixture;

	auto scroller = fixture.MakeScroller();

	fixture.eventDispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 800, 600 }));

	CHECK_NOTHROW(scroller->Draw());
}


TEST_CASE("Update advances scrollOffset by scrollSpeed * deltaTime exactly once per call")
{
	// Regression: Update used to add scrollSpeed * deltaTime twice per frame,
	// silently doubling the effective scroll speed.
	ImageScrollerFixture fixture;

	auto scroller = fixture.MakeScroller(ScrollDirection::HORIZONTAL, /*scrollSpeed*/ 10.0f);

	scroller->Update(1.0f);

	CHECK(scroller->GetScrollOffset() == doctest::Approx(10.0f));
}

TEST_CASE("Update wraps scrollOffset back below the tile width once it overshoots (horizontal)")
{
	ImageScrollerFixture fixture;

	fixture.textureBackend->nextSize = Vector2I{ 64, 64 };
	auto scroller = fixture.MakeScroller(ScrollDirection::HORIZONTAL, /*scrollSpeed*/ 100.0f);

	scroller->Update(1.0f);

	CHECK(scroller->GetScrollOffset() == doctest::Approx(36.0f)); // 100 - 64
}

TEST_CASE("Update wraps scrollOffset back below the tile height once it overshoots (vertical)")
{
	ImageScrollerFixture fixture;

	fixture.textureBackend->nextSize = Vector2I{ 64, 64 };
	auto scroller = fixture.MakeScroller(ScrollDirection::VERTICAL, /*scrollSpeed*/ 100.0f);

	scroller->Update(1.0f);

	CHECK(scroller->GetScrollOffset() == doctest::Approx(36.0f)); // 100 - 64
}

TEST_CASE("Update fully re-wraps scrollOffset when it overshoots by more than one tile in a single frame")
{
	ImageScrollerFixture fixture;

	fixture.textureBackend->nextSize = Vector2I{ 64, 64 };
	auto scroller = fixture.MakeScroller(ScrollDirection::HORIZONTAL, /*scrollSpeed*/ 1000.0f);

	scroller->Update(1.0f);

	auto offset = scroller->GetScrollOffset();

	CHECK(offset >= 0.0f);
	CHECK(offset < 64.0f);
	CHECK(offset == doctest::Approx(40.0f)); // 1000 mod 64
}

TEST_CASE("Update does not hang when the scaled tile size is zero")
{
	ImageScrollerFixture fixture;

	fixture.textureBackend->nextSize = Vector2I{ 0, 64 };
	auto scroller = fixture.MakeScroller(ScrollDirection::HORIZONTAL, /*scrollSpeed*/ 100.0f);

	CHECK_NOTHROW(scroller->Update(1.0f));
}


TEST_SUITE_END();