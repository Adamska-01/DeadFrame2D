#include "Constants/TiledPropertyNames.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Rendering/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Mocks/Context/Systems/Physics/MockPhysicsBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Constants;
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

	struct TiledMapFixture
	{
		std::unique_ptr<PhysicsEngine2D> physicsEngine;

		MockPhysicsBackend* physicsBackend;

		MockTimeProvider timeProvider;

		std::unique_ptr<CoroutineScheduler> coroutineScheduler;

		std::shared_ptr<FakeSceneHandleProvider> scene;


		TiledMapFixture()
		{
			auto physicsMock = std::make_unique<MockPhysicsBackend>();
			physicsBackend = physicsMock.get();
			physicsEngine = std::make_unique<PhysicsEngine2D>(MakePhysicsConfig(), CollisionMasks{}, std::move(physicsMock));

			coroutineScheduler = std::make_unique<CoroutineScheduler>(&timeProvider);

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext
				{
					.coroutineScheduler = coroutineScheduler.get(),
					.physicsEngine = physicsEngine.get()
				});
		}
	};

	// 2x2 map, one collision layer, tiles at (0,0) and (1,1) set, the diagonal left empty.
	std::shared_ptr<TiledMap> MakeTwoTileMap(int tileSize = 16)
	{
		auto layer = TiledLayer{};
		layer.Data = { { 1, 0 }, { 0, 1 } };

		auto map = std::make_shared<TiledMap>();
		map->width = 2;
		map->height = 2;
		map->tileSize = tileSize;
		map->layers = { layer };

		return map;
	}
}


TEST_SUITE_BEGIN("TiledMapCompatibleCollider2D");


TEST_CASE("Init reads tile size and dimensions from the sibling renderer's tile map")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto renderer = owner->AddComponent<TiledMapCompatibleRenderer>(MakeTwoTileMap());
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();

	CHECK(collider->GetTileSize() == 16);
	CHECK(collider->GetTileMapDimensions().x == 2);
	CHECK(collider->GetTileMapDimensions().y == 2);
	CHECK(collider->GetCollisionLayers().size() == 1);
}

TEST_CASE("Update builds one fixture per non-zero tile, skipping empty ones")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	owner->AddComponent<TiledMapCompatibleRenderer>(MakeTwoTileMap());
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	// Tiles (0,0) and (1,1) are set; (0,1) and (1,0) are 0 and must be skipped
	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 2);
}

TEST_CASE("Each tile fixture is centered on its cell in pixel space")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto layer = TiledLayer{};
	layer.Data = { { 1 } };

	auto map = std::make_shared<TiledMap>();
	map->width = 1;
	map->height = 1;
	map->tileSize = 16;
	map->layers = { layer };

	owner->AddComponent<TiledMapCompatibleRenderer>(map);
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);

	auto* box = std::get_if<BoxShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);
	REQUIRE(box != nullptr);
	CHECK(box->halfExtents.x == doctest::Approx(8.0f));
	CHECK(box->halfExtents.y == doctest::Approx(8.0f));
	CHECK(box->center.x == doctest::Approx(8.0f));
	CHECK(box->center.y == doctest::Approx(8.0f));
}

TEST_CASE("Per-layer Tiled properties override the fixture's physics material")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	auto layer = TiledLayer{};
	layer.Data = { { 1 } };
	layer.Properties[std::string(TiledPropertyNames::DENSITY)] = TiledProperty{ .Value = 2.5f };
	layer.Properties[std::string(TiledPropertyNames::FRICTION)] = TiledProperty{ .Value = 0.1f };
	layer.Properties[std::string(TiledPropertyNames::IS_SENSOR)] = TiledProperty{ .Value = true };
	layer.Properties[std::string(TiledPropertyNames::RESTITUTION)] = TiledProperty{ .Value = 0.8f };
	layer.Properties[std::string(TiledPropertyNames::RESTITUTION_THRESHOLD)] = TiledProperty{ .Value = 0.5f };

	auto map = std::make_shared<TiledMap>();
	map->width = 1;
	map->height = 1;
	map->tileSize = 16;
	map->layers = { layer };

	owner->AddComponent<TiledMapCompatibleRenderer>(map);
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	const auto& material = fixture.physicsBackend->lastPhysicsMaterial;
	CHECK(material.density == doctest::Approx(2.5f));
	CHECK(material.friction == doctest::Approx(0.1f));
	CHECK(material.isSensor);
	CHECK(material.restitution == doctest::Approx(0.8f));
	CHECK(material.restitutionThreshold == doctest::Approx(0.5f));
}

TEST_CASE("A second Update does not rebuild while not dirty, and does not leak destroyed fixtures")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	owner->AddComponent<TiledMapCompatibleRenderer>(MakeTwoTileMap());
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 2);
	CHECK(fixture.physicsBackend->destroyedFixtures.empty());

	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == 2);
	CHECK(fixture.physicsBackend->destroyedFixtures.empty());
}

TEST_CASE("Removing the collider destroys every tile fixture it built")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	owner->AddComponent<TiledMapCompatibleRenderer>(MakeTwoTileMap());
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 2);

	owner->RemoveComponent(collider);

	CHECK(fixture.physicsBackend->destroyedFixtures.size() == 2);
}

TEST_CASE("A collider whose RigidBody2D disappears drops its stale fixture ids instead of re-destroying them on a later rebuild")
{
	TiledMapFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	owner->AddComponent<TiledMapCompatibleRenderer>(MakeTwoTileMap());
	auto rigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	rigidBody->Init();

	auto collider = owner->AddComponent<TiledMapCompatibleCollider2D>();
	collider->Init();
	collider->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 2);

	// The RigidBody2D goes away, taking its fixtures with it; the collider is marked dirty
	// again but has nothing to search for
	owner->RemoveComponent(rigidBody);
	collider->SetIsTrigger(true);
	collider->Update(0.016f);

	// Nothing was (or could be) destroyed through the backend, and nothing was rebuilt
	CHECK(fixture.physicsBackend->destroyedFixtures.empty());
	CHECK(fixture.physicsBackend->createdFixtures.size() == 2);

	// A fresh RigidBody2D appears; the still-dirty collider rebuilds cleanly: the stale ids
	// from the dead body are dropped silently (never replayed against the backend), and two
	// fresh fixtures are created for the two tiles.
	auto newRigidBody = owner->AddComponent<RigidBody2D>(BodyDefinition2D{});
	newRigidBody->Init();

	collider->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == 4);
	CHECK(fixture.physicsBackend->destroyedFixtures.empty());
}


TEST_SUITE_END();