#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Collisions/BoxCollider2D.h"
#include "Engine/ECS/Component/Collisions/CircleCollider2D.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Context/Systems/Physics/MockPhysicsBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;
using namespace DF2D::Models;


namespace
{
	PhysicsConfig MakePhysicsConfig()
	{
		return PhysicsConfig
		{
			.gravityX = 0.0f,
			.gravityY = 9.81f,
			.velocityIterations = 6,
			.positionIterations = 2,
			.pixelPerMeter = 40.0f,
			.meterPerPixel = 1.0f / 40.0f,
			.debugDrawEnabled = false
		};
	}

	CollisionMasks MakeCollisionMasks()
	{
		return CollisionMasks{};
	}

	struct ColliderFixture
	{
		std::unique_ptr<PhysicsEngine2D> physicsEngine;

		MockPhysicsBackend* physicsBackend;

		MockTimeProvider timeProvider;

		std::unique_ptr<CoroutineScheduler> coroutineScheduler;

		std::shared_ptr<FakeSceneHandleProvider> scene;


		ColliderFixture()
		{
			auto physicsMock = std::make_unique<MockPhysicsBackend>();
			physicsBackend = physicsMock.get();
			physicsEngine = std::make_unique<PhysicsEngine2D>(MakePhysicsConfig(), MakeCollisionMasks(), std::move(physicsMock));

			coroutineScheduler = std::make_unique<CoroutineScheduler>(&timeProvider);

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext
				{
					.coroutineScheduler = coroutineScheduler.get(),
					.physicsEngine = physicsEngine.get()
				});
		}
	};
}


TEST_SUITE_BEGIN("Collider2D");


TEST_CASE("A collider added after its RigidBody2D builds a fixture on the first Update")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(32.0f, 16.0f));
	collider->Init();

	CHECK(fixture.physicsBackend->createdFixtures.empty());

	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);

	auto* box = std::get_if<BoxShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);
	REQUIRE(box != nullptr);
	CHECK(box->halfExtents.x == doctest::Approx(16.0f));
	CHECK(box->halfExtents.y == doctest::Approx(8.0f));
}

TEST_CASE("Update only rebuilds the fixture while dirty")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(32.0f, 32.0f));
	collider->Init();

	collider->Update(0.016f);
	CHECK(fixture.physicsBackend->createdFixtures.size() == 1);

	// Not dirty anymore: a second Update must not touch the backend again
	collider->Update(0.016f);
	CHECK(fixture.physicsBackend->createdFixtures.size() == 1);
}

TEST_CASE("SetNewBoxShape marks the collider dirty and rebuild destroys the old fixture")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(32.0f, 32.0f));
	collider->Init();
	collider->Update(0.016f);

	auto firstFixture = fixture.physicsBackend->createdFixtures.back();

	collider->SetNewBoxShape(Vector2F(64.0f, 8.0f), Vector2F::Zero, 0.0f);
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->destroyedFixtures.size() == 1);
	CHECK(fixture.physicsBackend->destroyedFixtures[0] == firstFixture);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 2);

	auto* box = std::get_if<BoxShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);
	REQUIRE(box != nullptr);
	CHECK(box->halfExtents.x == doctest::Approx(32.0f));
	CHECK(box->halfExtents.y == doctest::Approx(4.0f));
}

TEST_CASE("SetIsTrigger updates IsTrigger immediately and the flag reaches the next rebuilt fixture")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<CircleCollider2D>(8.0f);
	collider->Init();
	collider->Update(0.016f);

	CHECK_FALSE(collider->IsTrigger());

	collider->SetIsTrigger(true);

	// The flag itself flips synchronously, ahead of the deferred rebuild
	CHECK(collider->IsTrigger());

	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->lastPhysicsMaterial.isSensor);
}

TEST_CASE("A collider with no RigidBody2D never creates a fixture, but keeps retrying")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(16.0f, 16.0f));
	collider->Init();

	collider->Update(0.016f);
	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.empty());

	// A RigidBody2D shows up later: the still-dirty collider picks it up on the next Update
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == 1);
}

TEST_CASE("Removing the collider destroys its fixture")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(16.0f, 16.0f));
	collider->Init();
	collider->Update(0.016f);

	auto builtFixture = fixture.physicsBackend->createdFixtures.back();

	owner->RemoveComponent(collider);

	REQUIRE(fixture.physicsBackend->destroyedFixtures.size() == 1);
	CHECK(fixture.physicsBackend->destroyedFixtures[0] == builtFixture);
}

TEST_CASE("CircleCollider2D builds a circle shape sized to its radius and offset")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<CircleCollider2D>(12.0f, Vector2F(4.0f, -2.0f));
	collider->Init();
	collider->Update(0.016f);

	auto* circle = std::get_if<CircleShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);
	REQUIRE(circle != nullptr);
	CHECK(circle->radius == doctest::Approx(12.0f));
	CHECK(circle->center.x == doctest::Approx(4.0f));
	CHECK(circle->center.y == doctest::Approx(-2.0f));
}

TEST_CASE("A collider whose RigidBody2D disappears drops its stale fixture id instead of re-destroying it on a later rebuild")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<BoxCollider2D>(Vector2F(16.0f, 16.0f));
	collider->Init();
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);

	// The RigidBody2D goes away, taking its fixture with it; the collider is marked dirty
	// again but has nothing to search for
	owner->RemoveComponent(rigidBody);
	collider->SetIsTrigger(true);
	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->destroyedFixtures.empty());
	CHECK(fixture.physicsBackend->createdFixtures.size() == 1);

	// A fresh RigidBody2D appears; the still-dirty collider rebuilds cleanly: the stale id from
	// the dead body is dropped silently (never replayed against the backend), and one fresh
	// fixture is created.
	auto newRigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	newRigidBody->Init();

	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == 2);
	CHECK(fixture.physicsBackend->destroyedFixtures.empty());
}

TEST_CASE("GetPhysicsMaterial reflects the configured material and is queryable through a const reference")
{
	ColliderFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto materialTemplate = PhysicsMaterial{ .friction = 0.9f, .isSensor = true };
	auto collider = owner->AddComponent<CircleCollider2D>(4.0f, Vector2F::Zero, materialTemplate);

	const CircleCollider2D& constCollider = *collider;

	CHECK(constCollider.GetPhysicsMaterial().friction == doctest::Approx(0.9f));
	CHECK(constCollider.GetPhysicsMaterial().isSensor);
}


TEST_SUITE_END();