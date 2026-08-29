#include "Constants/MathConstants.h"
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Core/Math/MathUtils.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Context/Systems/Physics/MockPhysicsBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Constants;
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

	struct RigidBodyFixture
	{
		std::unique_ptr<PhysicsEngine2D> physicsEngine;

		MockPhysicsBackend* physicsBackend;

		MockTimeProvider timeProvider;

		std::unique_ptr<CoroutineScheduler> coroutineScheduler;

		std::shared_ptr<FakeSceneHandleProvider> scene;


		RigidBodyFixture()
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


		void TickCoroutines(float deltaTime = 0.016f)
		{
			static_cast<ICoreSystem*>(coroutineScheduler.get())->PreUpdate(deltaTime);
		}
	};
}


TEST_SUITE_BEGIN("RigidBody2D");


TEST_CASE("Constructor stores the body definition without touching the physics engine")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	ComponentHandle<RigidBody2D> rigidBody;

	CHECK_NOTHROW(rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{ .type = BodyType2D::Dynamic }));

	CHECK(fixture.physicsBackend->createdBodies.empty());

	CHECK_NOTHROW(rigidBody->Init());

	CHECK(fixture.physicsBackend->createdBodies.size() == 1);
	CHECK(fixture.physicsBackend->lastBodyDefinition.type == BodyType2D::Dynamic);
}

TEST_CASE("Init places the new body at the GameObject's current world transform")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	owner->GetTransform()->SetWorldPosition(Vector2F(64.0f, 32.0f));
	owner->GetTransform()->SetWorldRotation(90.0f);

	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	CHECK(fixture.physicsBackend->lastBodyPosition.x == doctest::Approx(64.0f));
	CHECK(fixture.physicsBackend->lastBodyPosition.y == doctest::Approx(32.0f));
	CHECK(fixture.physicsBackend->lastBodyAngle == doctest::Approx(MathUtils::ToRadians(90.0f)));
}

TEST_CASE("Removing the component destroys the body it created")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto builtBody = fixture.physicsBackend->createdBodies.back();

	owner->RemoveComponent(rigidBody);

	REQUIRE(fixture.physicsBackend->destroyedBodies.size() == 1);
	CHECK(fixture.physicsBackend->destroyedBodies[0] == builtBody);
}

TEST_CASE("Removing a component that was never Init'd does not destroy a body")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});

	owner->RemoveComponent(rigidBody);

	CHECK(fixture.physicsBackend->destroyedBodies.empty());
}

TEST_CASE("LateUpdate pulls the simulated body transform into the GameObject when nothing else moved it")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	fixture.physicsBackend->bodyTransformResult = BodyTransform2D
	{
		.position = Vector2F(150.0f, -20.0f),
		.angle = MathConstants::PI_f / 2.0f
	};

	rigidBody->LateUpdate(0.016f);

	CHECK(owner->GetTransform()->GetWorldPosition().x == doctest::Approx(150.0f));
	CHECK(owner->GetTransform()->GetWorldPosition().y == doctest::Approx(-20.0f));
	CHECK(owner->GetTransform()->GetWorldRotation() == doctest::Approx(90.0f));
}

TEST_CASE("LateUpdate pushes a manual transform change onto the body instead of being overwritten by it")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	// Simulation is at rest at the origin; gameplay code teleports the object directly.
	owner->GetTransform()->SetWorldPosition(Vector2F(500.0f, 10.0f));

	rigidBody->LateUpdate(0.016f);

	CHECK(fixture.physicsBackend->lastBodyPosition.x == doctest::Approx(500.0f));
	CHECK(fixture.physicsBackend->lastBodyPosition.y == doctest::Approx(10.0f));
}

TEST_CASE("ChangeBodyType defers the backend call until the next Update")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	rigidBody->ChangeBodyType(BodyType2D::Kinematic);

	// Not applied yet: the pending action only runs on Update
	CHECK(fixture.physicsBackend->lastBodyType != BodyType2D::Kinematic);

	rigidBody->Update(0.016f);

	CHECK(fixture.physicsBackend->lastBodyType == BodyType2D::Kinematic);
}

TEST_CASE("CreateFixture and DestroyFixture delegate to the physics engine for this body")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto builtBody = fixture.physicsBackend->createdBodies.back();

	auto fixtureId = rigidBody->CreateFixture(PhysicsMaterial{}, ComponentHandle<ContactEventProvider>{});

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);
	CHECK(fixture.physicsBackend->lastFixtureBody == builtBody);
	CHECK(fixtureId == fixture.physicsBackend->createdFixtures.back());

	rigidBody->DestroyFixture(fixtureId);

	REQUIRE(fixture.physicsBackend->destroyedFixtures.size() == 1);
	CHECK(fixture.physicsBackend->destroyedFixtures[0] == fixtureId);
}

TEST_CASE("Velocity getters and setters pass straight through to the physics engine")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	fixture.physicsBackend->linearVelocityResult = Vector2F(3.0f, 4.0f);
	CHECK(rigidBody->GetVelocity().x == doctest::Approx(3.0f));
	CHECK(rigidBody->GetVelocity().y == doctest::Approx(4.0f));

	rigidBody->SetVelocity(Vector2F(1.0f, 2.0f));
	CHECK(fixture.physicsBackend->lastLinearVelocity.x == doctest::Approx(1.0f));
	CHECK(fixture.physicsBackend->lastLinearVelocity.y == doctest::Approx(2.0f));

	rigidBody->SetVelocityX(10.0f);
	CHECK(fixture.physicsBackend->lastLinearVelocity.x == doctest::Approx(10.0f));
	CHECK(fixture.physicsBackend->lastLinearVelocity.y == doctest::Approx(4.0f));

	rigidBody->SetVelocityY(20.0f);
	CHECK(fixture.physicsBackend->lastLinearVelocity.x == doctest::Approx(3.0f));
	CHECK(fixture.physicsBackend->lastLinearVelocity.y == doctest::Approx(20.0f));
}

TEST_CASE("Impulse, force, and gravity scale pass straight through to the physics engine")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	rigidBody->AddImpulse(Vector2F(5.0f, 6.0f));
	CHECK(fixture.physicsBackend->lastImpulse.x == doctest::Approx(5.0f));
	CHECK(fixture.physicsBackend->lastImpulse.y == doctest::Approx(6.0f));

	rigidBody->AddImpulseX(7.0f);
	CHECK(fixture.physicsBackend->lastImpulse.x == doctest::Approx(7.0f));
	CHECK(fixture.physicsBackend->lastImpulse.y == doctest::Approx(0.0f));

	rigidBody->AddImpulseY(8.0f);
	CHECK(fixture.physicsBackend->lastImpulse.x == doctest::Approx(0.0f));
	CHECK(fixture.physicsBackend->lastImpulse.y == doctest::Approx(8.0f));

	rigidBody->AddForce(Vector2F(9.0f, -1.0f));
	CHECK(fixture.physicsBackend->lastForce.x == doctest::Approx(9.0f));
	CHECK(fixture.physicsBackend->lastForce.y == doctest::Approx(-1.0f));

	rigidBody->SetGravityScale(0.5f);
	CHECK(fixture.physicsBackend->lastGravityScale == doctest::Approx(0.5f));
}

TEST_CASE("Deactivating the owning GameObject defers SetBodyEnabled until the next coroutine tick")
{
	RigidBodyFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	fixture.physicsBackend->lastBodyEnabled = true;

	owner->SetActive(false);

	// The world may be mid-callback; the disable must not happen synchronously
	CHECK(fixture.physicsBackend->lastBodyEnabled);

	fixture.TickCoroutines();

	CHECK_FALSE(fixture.physicsBackend->lastBodyEnabled);
}


TEST_SUITE_END();