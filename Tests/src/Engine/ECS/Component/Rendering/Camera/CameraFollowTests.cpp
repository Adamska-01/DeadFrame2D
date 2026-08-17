#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Component/Rendering/Camera/CameraFollow.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
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
	struct CameraFollowFixture
	{
		EventDispatcher eventDispatcher;

		std::unique_ptr<Renderer> renderer;

		MockRenderBackend* renderBackend;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> cameraOwner;

		ComponentHandle<Camera> camera;

		ObjectHandle<TestGameObject> targetOwner;

		ObjectHandle<TestGameObject> followerOwner;

		ComponentHandle<CameraFollow> follow;


		CameraFollowFixture()
		{
			auto rb = std::make_unique<MockRenderBackend>();
			renderBackend = rb.get();
			renderer = std::make_unique<Renderer>(std::move(rb));

			scene = std::make_shared<TestScene>(&eventDispatcher);
			scene->SetCoreContext(CoreContext{ .renderer = renderer.get() });

			cameraOwner = scene->Spawn<TestGameObject>();
			camera = cameraOwner->AddComponent<Camera>();
			camera->Init();

			targetOwner = scene->Spawn<TestGameObject>();

			followerOwner = scene->Spawn<TestGameObject>();
			follow = followerOwner->AddComponent<CameraFollow>(camera, targetOwner);
			follow->Init();
		}
	};
}


TEST_SUITE_BEGIN("CameraFollow");


TEST_CASE("Init throws when CoreContext has no Renderer")
{
	EventDispatcher eventDispatcher;
	auto scene = std::make_shared<TestScene>(&eventDispatcher);

	auto owner = scene->Spawn<TestGameObject>();
	auto follow = owner->AddComponent<CameraFollow>(ComponentHandle<Camera>{}, ObjectHandle<GameObject>{});

	CHECK_THROWS_AS(follow->Init(), std::invalid_argument);
}

TEST_CASE("Init throws when CoreContext/ServiceContext has no EventDispatcher")
{
	auto rb = std::make_unique<MockRenderBackend>();
	auto renderer = std::make_unique<Renderer>(std::move(rb));

	auto scene = std::make_shared<FakeSceneHandleProvider>();
	scene->SetCoreContext(CoreContext{ .renderer = renderer.get() });

	auto owner = scene->Create<TestGameObject>();
	auto follow = owner->AddComponent<CameraFollow>(ComponentHandle<Camera>{}, ObjectHandle<GameObject>{});

	CHECK_THROWS_AS(follow->Init(), std::invalid_argument);
}

TEST_CASE("Update does nothing when the target handle is invalid")
{
	CameraFollowFixture fixture;

	fixture.follow->SetTarget(ObjectHandle<GameObject>{});

	CHECK_NOTHROW(fixture.follow->Update(1.0f));
	CHECK(fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition() == Vector2F::Zero);
}

TEST_CASE("Update moves the camera toward the target using the follow speed as a lerp factor")
{
	CameraFollowFixture fixture;

	fixture.targetOwner->GetTransform()->SetWorldPosition(Vector2F{ 100.0f, 0.0f });
	fixture.follow->SetFollowSpeed(0.5f);

	fixture.follow->Update(1.0f); // t = clamp(0.5 * 1, 0, 1) = 0.5

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(50.0f));
	CHECK(pos.y == doctest::Approx(0.0f));
}

TEST_CASE("Update reaches the target directly once followSpeed * deltaTime clamps to 1")
{
	CameraFollowFixture fixture;

	fixture.targetOwner->GetTransform()->SetWorldPosition(Vector2F{ 100.0f, -20.0f });
	fixture.follow->SetFollowSpeed(100.0f);

	fixture.follow->Update(1.0f);

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(100.0f));
	CHECK(pos.y == doctest::Approx(-20.0f));
}

TEST_CASE("SetOffset shifts the desired center away from the target position")
{
	CameraFollowFixture fixture;

	fixture.targetOwner->GetTransform()->SetWorldPosition(Vector2F{ 100.0f, 0.0f });
	fixture.follow->SetOffset(Vector2F{ 10.0f, 5.0f });
	fixture.follow->SetFollowSpeed(100.0f);

	fixture.follow->Update(1.0f);

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(110.0f));
	CHECK(pos.y == doctest::Approx(5.0f));
}

TEST_CASE("SetBounds clamps the desired center to the world bounds' midpoint when the world is smaller than the view")
{
	CameraFollowFixture fixture;

	// Default mock resolution (1920x1080) makes the view far bigger than these bounds,
	// so the desired center collapses to the bounds' midpoint regardless of target position.
	fixture.follow->SetBounds(RectF{ -50.0f, -50.0f, 100.0f, 100.0f });
	fixture.targetOwner->GetTransform()->SetWorldPosition(Vector2F{ 1000.0f, 1000.0f });
	fixture.follow->SetFollowSpeed(100.0f);

	fixture.follow->Update(1.0f);

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(0.0f));
	CHECK(pos.y == doctest::Approx(0.0f));
}

TEST_CASE("SetBounds clamps the desired center to the near edge when the world is larger than the view")
{
	CameraFollowFixture fixture;

	// Shrink the view (200x200) below the world bounds (1000x1000) so the target
	// actually gets clamped to an edge instead of collapsing to the midpoint.
	fixture.eventDispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 200, 200 }));

	fixture.follow->SetBounds(RectF{ -500.0f, -500.0f, 1000.0f, 1000.0f });
	fixture.targetOwner->GetTransform()->SetWorldPosition(Vector2F{ 1000.0f, 0.0f });
	fixture.follow->SetFollowSpeed(100.0f);

	fixture.follow->Update(1.0f);

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(400.0f)); // xMax = -500 + 1000 - halfView(100) = 400
	CHECK(pos.y == doctest::Approx(0.0f));
}

TEST_CASE("SetTarget switches which object is followed")
{
	CameraFollowFixture fixture;

	auto otherTargetOwner = fixture.scene->Spawn<TestGameObject>();
	otherTargetOwner->GetTransform()->SetWorldPosition(Vector2F{ -30.0f, 40.0f });

	fixture.follow->SetTarget(otherTargetOwner);
	fixture.follow->SetFollowSpeed(100.0f);

	fixture.follow->Update(1.0f);

	auto pos = fixture.camera->GetGameObject()->GetTransform()->GetWorldPosition();
	CHECK(pos.x == doctest::Approx(-30.0f));
	CHECK(pos.y == doctest::Approx(40.0f));
}


TEST_SUITE_END();