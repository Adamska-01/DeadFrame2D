#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Audio/AudioListener.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Context/Systems/Physics/MockPhysicsBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
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
		return CollisionMasks
		{
			.categories =
			{
				NamedBitflag{ .name = "AUDIO", .value = 2 }
			}
		};
	}

	// Wires a PhysicsEngine2D against a mock backend behind a fake scene, mirroring
	// what SystemInitializer/Scene provide via CoreContext.
	struct AudioListenerFixture
	{
		std::unique_ptr<PhysicsEngine2D> physicsEngine;

		MockPhysicsBackend* physicsBackend;

		std::shared_ptr<FakeSceneHandleProvider> scene;


		AudioListenerFixture()
		{
			auto physicsMock = std::make_unique<MockPhysicsBackend>();
			physicsBackend = physicsMock.get();
			physicsEngine = std::make_unique<PhysicsEngine2D>(MakePhysicsConfig(), MakeCollisionMasks(), std::move(physicsMock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .physicsEngine = physicsEngine.get() });
		}
	};
}


TEST_SUITE_BEGIN("AudioListener");


TEST_CASE("Update builds a kinematic sensor circle filtered to AUDIO mask")
{
	AudioListenerFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<AudioListener>();
	listener->Init();

	listener->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdBodies.size() == 1);
	CHECK(fixture.physicsBackend->lastBodyDefinition.type == BodyType2D::Kinematic);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.isSensor);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.filter.categoryBits == 2);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.filter.maskBits == 2);

	auto* circle = std::get_if<CircleShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);

	REQUIRE(circle != nullptr);
}

TEST_CASE("LateUpdate moves the body and wakes it when the transform changes")
{
	AudioListenerFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<AudioListener>();
	listener->Init();
	listener->Update(0.016f);

	owner->GetTransform()->SetWorldPosition(Vector2F(64.0f, -32.0f));

	listener->LateUpdate(0.016f);

	CHECK(fixture.physicsBackend->lastBodyPosition.x == doctest::Approx(64.0f));
	CHECK(fixture.physicsBackend->lastBodyPosition.y == doctest::Approx(-32.0f));
	CHECK(fixture.physicsBackend->lastBodyAwake == true);
}

TEST_CASE("LateUpdate is a no-op when the transform has not changed")
{
	AudioListenerFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<AudioListener>();
	listener->Init();
	listener->Update(0.016f);

	// RebuildFixture already synced the body to the transform; nothing moved since
	fixture.physicsBackend->lastBodyAwake = false;
	fixture.physicsBackend->lastBodyOpTarget = 0;

	listener->LateUpdate(0.016f);

	CHECK(fixture.physicsBackend->lastBodyOpTarget == 0);
	CHECK(fixture.physicsBackend->lastBodyAwake == false);
}

TEST_CASE("Removing the component destroys its physics body")
{
	AudioListenerFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<AudioListener>();
	listener->Init();
	listener->Update(0.016f);

	owner->RemoveComponent(listener);

	CHECK(fixture.physicsBackend->destroyedBodies.size() == 1);
}


TEST_SUITE_END();