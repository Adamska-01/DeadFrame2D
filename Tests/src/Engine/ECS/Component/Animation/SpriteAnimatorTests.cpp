#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Data/Components/Animation/SpriteAnimationProperties.h"
#include "Engine/ECS/Component/Animation/SpriteAnimator.h"
#include "Engine/ECS/Component/Rendering/SpriteRenderer.h"
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
	// Fully wires a TestGameObject with SpriteRenderer + SpriteAnimator against a
	// mock-backed TextureManager, mirroring what Scene normally does via Init().
	struct SpriteAnimatorFixture
	{
		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* backend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;

		ComponentHandle<SpriteRenderer> spriteRenderer;

		ComponentHandle<SpriteAnimator> animator;


		SpriteAnimatorFixture()
		{
			auto mock = std::make_unique<MockTextureBackend>();
			backend = mock.get();
			textureManager = std::make_unique<TextureManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

			owner = scene->Create<TestGameObject>();

			spriteRenderer = owner->AddComponent<SpriteRenderer>("");
			animator = owner->AddComponent<SpriteAnimator>();

			spriteRenderer->Init();
			animator->Init();
		}
	};


	SpriteAnimationProperties MakeProps(
		std::string name,
		std::string_view spriteSource,
		int columnCount,
		int rowCount = 1,
		int sourceRowNumber = 0,
		float animSpeed = 1.0f,
		bool loop = false)
	{
		return SpriteAnimationProperties
		{
			.name = std::move(name),
			.spriteSource = spriteSource,
			.columnCount = columnCount,
			.rowCount = rowCount,
			.sourceRowNumber = sourceRowNumber,
			.animSpeed = animSpeed,
			.loop = loop
		};
	}
}


TEST_SUITE_BEGIN("SpriteAnimator");


TEST_CASE("A SpriteRenderer added after Init() binds reactively via OnNewComponentAddedHandler")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto backend = mock.get();
	auto textureManager = std::make_unique<TextureManager>(std::move(mock));

	auto scene = std::make_shared<FakeSceneHandleProvider>();
	scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

	auto owner = scene->Create<TestGameObject>();

	auto animator = owner->AddComponent<SpriteAnimator>();

	animator->Init();
	animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 1.0f, /*loop*/ true));
	animator->PlayAnimation("Idle");

	// SpriteRenderer still doesn't exist yet: nothing to push the frame onto.
	auto spriteRenderer = owner->AddComponent<SpriteRenderer>("");

	// AddComponent fires OnNewComponentAddedHandler synchronously, binding `sprite` —
	// but SpriteRenderer::Init() (deferred by Scene in real usage) hasn't run yet, so
	// the reactive LoadSprite silently no-ops at this point.
	CHECK(spriteRenderer->GetTexture() == 0);

	spriteRenderer->Init();

	// Nothing re-triggers a load automatically; Update() is what retries and catches up.
	animator->Update(0.0f);

	CHECK(spriteRenderer->GetTexture() != 0);
	CHECK(backend->lastLoadedFile == "idle.png");
	CHECK(spriteRenderer->GetSourceRect().has_value());
}

TEST_CASE("Init throws when CoreContext has no TextureManager")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	owner->AddComponent<SpriteRenderer>("");
	auto animator = owner->AddComponent<SpriteAnimator>();

	CHECK_THROWS_AS(animator->Init(), std::invalid_argument);
}

TEST_CASE("Init leaves the SpriteRenderer active, since it is now the one that draws")
{
	SpriteAnimatorFixture fixture;

	CHECK(fixture.spriteRenderer->IsActive());
}

TEST_CASE("AddAnimation/PlayAnimation called before Init (blueprint constructor order) do not crash, and Init loads the texture")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto backend = mock.get();
	auto textureManager = std::make_unique<TextureManager>(std::move(mock));

	auto scene = std::make_shared<FakeSceneHandleProvider>();
	scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

	auto owner = scene->Create<TestGameObject>();

	auto spriteRenderer = owner->AddComponent<SpriteRenderer>("");
	auto animator = owner->AddComponent<SpriteAnimator>();

	// Mirrors Coin/Key/Enemy blueprint constructors: AddAnimation + PlayAnimation
	// run immediately after AddComponent<SpriteAnimator>(), before Scene ever calls
	// Init() on it, so `sprite` is still an unwired ComponentHandle here.
	auto props = MakeProps("CoinFlip", "coin.png", 8, 1, 0, 5.0f, /*loop*/ true);

	CHECK_NOTHROW(animator->AddAnimation(props));
	CHECK_NOTHROW(animator->PlayAnimation(props.name));

	spriteRenderer->Init();

	CHECK_NOTHROW(animator->Init());

	CHECK(spriteRenderer->GetTexture() != 0);
	CHECK(backend->lastLoadedFile == "coin.png");
}


TEST_CASE("AddAnimation rejects an animation with an empty name")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("", "a.png", 4));

	CHECK(fixture.animator->GetCurrentAnimationProperties() == nullptr);
}

TEST_CASE("AddAnimation rejects a non-positive columnCount")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 0));

	CHECK(fixture.animator->GetCurrentAnimationProperties() == nullptr);
}

TEST_CASE("AddAnimation rejects a non-positive rowCount")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4, 0));

	CHECK(fixture.animator->GetCurrentAnimationProperties() == nullptr);
}

TEST_CASE("AddAnimation rejects a sourceRowNumber outside [0, rowCount)")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4, 2, 2));
	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4, 2, -1));

	CHECK(fixture.animator->GetCurrentAnimationProperties() == nullptr);
}

TEST_CASE("AddAnimation accepts valid properties and makes the first animation current")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4));

	REQUIRE(fixture.animator->GetCurrentAnimationProperties() != nullptr);
	CHECK(fixture.animator->GetCurrentAnimationProperties()->name == "Idle");
	CHECK(fixture.animator->IsPlaying("Idle"));
}

TEST_CASE("AddAnimation does not change the current animation once one is already playing")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4));
	fixture.animator->AddAnimation(MakeProps("Run", "b.png", 6));

	CHECK(fixture.animator->IsPlaying("Idle"));
}


TEST_CASE("PlayAnimation on an unknown name is a no-op")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "a.png", 4));
	fixture.animator->PlayAnimation("DoesNotExist");

	CHECK(fixture.animator->IsPlaying("Idle"));
}

TEST_CASE("PlayAnimation switches the current animation and loads its sprite")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4));
	fixture.animator->AddAnimation(MakeProps("Run", "run.png", 6));

	fixture.animator->PlayAnimation("Run");

	CHECK(fixture.animator->IsPlaying("Run"));
	CHECK(fixture.spriteRenderer->GetTexture() != 0);
	CHECK(fixture.backend->lastLoadedFile == "run.png");
}

TEST_CASE("PlayAnimation without restartIfPlaying keeps progress when replaying the current animation")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 1.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");
	fixture.animator->Update(2.0f);

	auto progressBefore = fixture.animator->GetAnimationProgressRatio();

	fixture.animator->PlayAnimation("Idle");

	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(progressBefore));
}

TEST_CASE("PlayAnimation with restartIfPlaying resets progress on the current animation")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 1.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");
	fixture.animator->Update(2.0f);

	fixture.animator->PlayAnimation("Idle", /*restartIfPlaying*/ true);

	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(0.0f));
}


TEST_CASE("Update advances currentFrame by animSpeed * dt")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 10, 1, 0, 2.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");

	fixture.animator->Update(1.0f);

	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(0.2f));
}

TEST_CASE("Update wraps a looping animation back to frame 0 once it reaches columnCount")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 1.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");

	fixture.animator->Update(4.5f);

	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(0.0f));
}

TEST_CASE("Update clamps a non-looping animation's rendered frame but reports full progress once finished")
{
	SpriteAnimatorFixture fixture;

	fixture.animator->AddAnimation(MakeProps("Hit", "hit.png", 3, 1, 0, 1.0f, /*loop*/ false));
	fixture.animator->PlayAnimation("Hit");

	fixture.animator->Update(5.0f);

	// Progress reports 100% complete even though the rendered frame clamps to columnCount - 1.
	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(1.0f));

	// Regression: a finished non-looping animation must not silently restart on later ticks.
	fixture.animator->Update(5.0f);
	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(1.0f));

	fixture.animator->Update(5.0f);
	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(1.0f));
}


TEST_CASE("GetFrameRect divides the texture size by column and row count")
{
	SpriteAnimatorFixture fixture;

	fixture.backend->nextSize = Vector2I{ 64, 32 };
	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 2));
	fixture.animator->PlayAnimation("Idle");

	auto frameRect = fixture.animator->GetFrameRect();

	CHECK(frameRect.w == 16);
	CHECK(frameRect.h == 16);
}

TEST_CASE("GetAnimationProgressRatio is 0 when no animation has been added")
{
	SpriteAnimatorFixture fixture;

	CHECK(fixture.animator->GetAnimationProgressRatio() == doctest::Approx(0.0f));
}

TEST_CASE("PlayAnimation pushes the starting frame onto the SpriteRenderer immediately")
{
	SpriteAnimatorFixture fixture;

	fixture.backend->nextSize = Vector2I{ 64, 16 };
	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 1.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");

	auto srcRect = fixture.spriteRenderer->GetSourceRect();

	REQUIRE(srcRect.has_value());
	CHECK(srcRect->x == 0);
	CHECK(srcRect->w == 16);
	CHECK(fixture.spriteRenderer->GetDrawSize().x == 16);
	CHECK(fixture.spriteRenderer->GetDrawSize().y == 16);
}

TEST_CASE("Update pushes the current frame's source rect onto the SpriteRenderer")
{
	SpriteAnimatorFixture fixture;

	fixture.backend->nextSize = Vector2I{ 64, 16 };
	fixture.animator->AddAnimation(MakeProps("Idle", "idle.png", 4, 1, 0, 4.0f, /*loop*/ true));
	fixture.animator->PlayAnimation("Idle");

	fixture.animator->Update(0.5f); // currentFrame = 4.0 * 0.5 = 2.0

	auto srcRect = fixture.spriteRenderer->GetSourceRect();

	REQUIRE(srcRect.has_value());
	CHECK(srcRect->x == 32); // frameWidth(16) * frameIndex(2)
	CHECK(srcRect->w == 16);
}


TEST_SUITE_END();