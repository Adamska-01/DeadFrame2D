#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Mocks/Context/Systems/Physics/FakeContactProvider.h"
#include "Mocks/Context/Systems/Physics/MockPhysicsBackend.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


static PhysicsConfig MakePhysicsConfig()
{
	auto config = PhysicsConfig
	{
		.gravityX = 0.0f,
		.gravityY = 9.81f,
		.velocityIterations = 6,
		.positionIterations = 2,
		.pixelPerMeter = 40.0f,
		.meterPerPixel = 1.0f / 40.0f
	};

	return config;
}


static CollisionMasks MakeCollisionMasks()
{
	return CollisionMasks
	{
		.categories =
		{
			NamedBitflag{ .name = "DEFAULT", .value = 1 },
			NamedBitflag{ .name = "AUDIO", .value = 2 },
			NamedBitflag{ .name = "ALL", .value = 65535 }
		}
	};
}


static std::unique_ptr<PhysicsEngine2D> MakePhysicsEngine(MockPhysicsBackend*& outMock)
{
	auto mock = std::make_unique<MockPhysicsBackend>();
	outMock = mock.get();

	return std::make_unique<PhysicsEngine2D>(MakePhysicsConfig(), MakeCollisionMasks(), std::move(mock));
}


TEST_SUITE_BEGIN("PhysicsEngine2D");


TEST_CASE("Constructor registers itself as the backend's contact sink")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	CHECK(mock->sink != nullptr);
}


TEST_CASE("EndUpdate steps the backend with the configured iterations")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	ICoreSystem* system = physicsEngine.get();

	auto deltaTime = 0.016f;

	system->EndUpdate(deltaTime);

	CHECK(mock->stepCount == 1);
	CHECK(mock->lastStepDeltaTime == doctest::Approx(deltaTime));
	CHECK(mock->lastVelocityIterations == 6);
	CHECK(mock->lastPositionIterations == 2);
}


TEST_CASE("BeginFrame and PreUpdate are safe no-ops")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	ICoreSystem* system = physicsEngine.get();

	system->BeginFrame();
	system->PreUpdate(0.016f);

	CHECK(mock->stepCount == 0);
	CHECK(mock->debugDrawCount == 0);
}


TEST_CASE("GetGravity reflects the configuration")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);
	auto config = MakePhysicsConfig();

	auto gravity = physicsEngine->GetGravity();

	CHECK(gravity.x == doctest::Approx(config.gravityX));
	CHECK(gravity.y == doctest::Approx(config.gravityY));
}


TEST_CASE("SetGravity delegates to the backend and updates the configuration")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	auto newGravity = Vector2F(1.0f, -5.0f);

	physicsEngine->SetGravity();

	CHECK(mock->setGravityCount == 1);
	CHECK(mock->lastGravity.x == doctest::Approx(newGravity.x));
	CHECK(mock->lastGravity.y == doctest::Approx(newGravity.y));

	auto gravity = physicsEngine->GetGravity();

	CHECK(gravity.x == doctest::Approx(newGravity.x));
	CHECK(gravity.y == doctest::Approx(newGravity.y));
}


TEST_CASE("CreateBody delegates and propagates the backend id")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	mock->nextBodyId = 7;

	auto bodyDefinition = BodyDefinition2D
	{
		.type = BodyType2D::Dynamic,
		.gravityScale = 0.5f
	};

	auto body = physicsEngine->CreateBody(bodyDefinition);

	CHECK(body == 7);
	CHECK(mock->createdBodies.size() == 1);
	CHECK(mock->lastBodyDefinition.type == BodyType2D::Dynamic);
	CHECK(mock->lastBodyDefinition.gravityScale == doctest::Approx(bodyDefinition.gravityScale));
}


TEST_CASE("CreateBody propagates backend failure as id 0")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	mock->createBodyFails = true;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	CHECK(body <= 0);
}


TEST_CASE("DestroyBody delegates to the backend and ignores id 0")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	physicsEngine->DestroyBody(body);

	CHECK(mock->destroyedBodies.size() == 1);
	CHECK(mock->destroyedBodies[0] == body);

	physicsEngine->DestroyBody(0);

	CHECK(mock->destroyedBodies.size() == 1);
}


TEST_CASE("CreateFixture delegates, propagates the id and forwards the material")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider provider;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	auto physicsMaterial = PhysicsMaterial
	{
		.friction = 0.7f,
		.isSensor = true
	};

	auto fixture = physicsEngine->CreateFixture(body, physicsMaterial, &provider);

	CHECK(fixture == 1);
	CHECK(mock->lastFixtureBody == body);
	CHECK(mock->lastPhysicsMaterial.friction == doctest::Approx(physicsMaterial.friction));
	CHECK(mock->lastPhysicsMaterial.isSensor == physicsMaterial.isSensor);
}


TEST_CASE("CreateFixture with body id 0 does not touch the backend")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider provider;

	auto fixture = physicsEngine->CreateFixture(0, PhysicsMaterial{}, &provider);

	CHECK(fixture == 0);
	CHECK(mock->createdFixtures.empty());
}


TEST_CASE("CreateFixture propagates backend failure as id 0")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider provider;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	mock->createFixtureFails = true;

	auto fixture = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &provider);

	CHECK(fixture == 0);
}


TEST_CASE("Contact begin routes mirrored collision info to both providers")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider providerA;
	FakeContactProvider providerB;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	auto fixtureA = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerA);
	auto fixtureB = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerB);

	mock->sink->OnContactBegin(fixtureA, fixtureB, Vector2F(10.0f, 20.0f), Vector2F(0.0f, 1.0f));

	REQUIRE(providerA.enterInfos.size() == 1);
	REQUIRE(providerB.enterInfos.size() == 1);

	CHECK(providerA.enterInfos[0].contactPoint.x == doctest::Approx(10.0f));
	CHECK(providerA.enterInfos[0].contactPoint.y == doctest::Approx(20.0f));
	CHECK(providerA.enterInfos[0].normal.x == doctest::Approx(0.0f));
	CHECK(providerA.enterInfos[0].normal.y == doctest::Approx(1.0f));

	// The second provider receives the same contact point with a negated normal
	CHECK(providerB.enterInfos[0].contactPoint.x == doctest::Approx(10.0f));
	CHECK(providerB.enterInfos[0].contactPoint.y == doctest::Approx(20.0f));
	CHECK(providerB.enterInfos[0].normal.x == doctest::Approx(0.0f));
	CHECK(providerB.enterInfos[0].normal.y == doctest::Approx(-1.0f));
}


TEST_CASE("Contact end routes zeroed collision info to both providers")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider providerA;
	FakeContactProvider providerB;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	auto fixtureA = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerA);
	auto fixtureB = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerB);

	mock->sink->OnContactEnd(fixtureA, fixtureB);

	REQUIRE(providerA.exitInfos.size() == 1);
	REQUIRE(providerB.exitInfos.size() == 1);

	CHECK(providerA.exitInfos[0].contactPoint.x == doctest::Approx(0.0f));
	CHECK(providerA.exitInfos[0].contactPoint.y == doctest::Approx(0.0f));
	CHECK(providerA.exitInfos[0].normal.x == doctest::Approx(0.0f));
	CHECK(providerA.exitInfos[0].normal.y == doctest::Approx(0.0f));
}


TEST_CASE("Contacts with unknown fixture ids are ignored")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider provider;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	auto fixture = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &provider);

	mock->sink->OnContactBegin(fixture, 999, Vector2F(1.0f, 1.0f), Vector2F(0.0f, 1.0f));
	mock->sink->OnContactEnd(999, fixture);

	CHECK(provider.enterInfos.empty());
	CHECK(provider.exitInfos.empty());
}


TEST_CASE("DestroyFixture purges the record before the backend call")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider providerA;
	FakeContactProvider providerB;

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	auto fixtureA = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerA);
	auto fixtureB = physicsEngine->CreateFixture(body, PhysicsMaterial{}, &providerB);

	// Emulate Box2D firing an end contact for the dying fixture during destruction
	mock->destroyFixtureFiresEndContact = true;
	mock->endContactOtherFixture = fixtureB;

	physicsEngine->DestroyFixture(fixtureA);

	CHECK(mock->destroyedFixtures.size() == 1);
	CHECK(mock->destroyedFixtures[0] == fixtureA);

	// The record was purged before the backend call, so no provider is invoked
	CHECK(providerA.exitInfos.empty());
	CHECK(providerB.exitInfos.empty());
}


TEST_CASE("DestroyFixture ignores id 0")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	physicsEngine->DestroyFixture(0);

	CHECK(mock->destroyedFixtures.empty());
}


TEST_CASE("DestroyBody purges all fixture records belonging to that body")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	FakeContactProvider providerA;
	FakeContactProvider providerB;
	FakeContactProvider providerC;

	auto bodyA = physicsEngine->CreateBody(BodyDefinition2D{});
	auto bodyB = physicsEngine->CreateBody(BodyDefinition2D{});

	auto fixtureA = physicsEngine->CreateFixture(bodyA, PhysicsMaterial{}, &providerA);
	auto fixtureB = physicsEngine->CreateFixture(bodyA, PhysicsMaterial{}, &providerB);
	auto fixtureC = physicsEngine->CreateFixture(bodyB, PhysicsMaterial{}, &providerC);

	physicsEngine->DestroyBody(bodyA);

	// Contacts referencing the destroyed body's fixtures are now ignored
	mock->sink->OnContactBegin(fixtureA, fixtureC, Vector2F(1.0f, 1.0f), Vector2F(0.0f, 1.0f));
	mock->sink->OnContactEnd(fixtureB, fixtureC);

	CHECK(providerA.enterInfos.empty());
	CHECK(providerB.exitInfos.empty());
	CHECK(providerC.enterInfos.empty());
	CHECK(providerC.exitInfos.empty());

	// The surviving body's fixture still routes contacts against other live fixtures
	auto fixtureD = physicsEngine->CreateFixture(bodyB, PhysicsMaterial{}, &providerA);

	mock->sink->OnContactBegin(fixtureC, fixtureD, Vector2F(2.0f, 2.0f), Vector2F(1.0f, 0.0f));

	CHECK(providerC.enterInfos.size() == 1);
	CHECK(providerA.enterInfos.size() == 1);
}


TEST_CASE("Body operations pass through to the backend")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	auto body = physicsEngine->CreateBody(BodyDefinition2D{});

	physicsEngine->SetBodyEnabled(body, false);
	CHECK(mock->lastBodyOpTarget == body);
	CHECK(mock->lastBodyEnabled == false);

	physicsEngine->SetBodyType(body, BodyType2D::Kinematic);
	CHECK(mock->lastBodyType == BodyType2D::Kinematic);

	physicsEngine->SetBodyTransform(body, Vector2F(64.0f, 32.0f), 1.5f);
	CHECK(mock->lastBodyPosition.x == doctest::Approx(64.0f));
	CHECK(mock->lastBodyPosition.y == doctest::Approx(32.0f));
	CHECK(mock->lastBodyAngle == doctest::Approx(1.5f));

	mock->bodyTransformResult = { Vector2F(128.0f, 16.0f), 0.5f };

	auto bodyTransform = physicsEngine->GetBodyTransform(body);
	CHECK(bodyTransform.position.x == doctest::Approx(128.0f));
	CHECK(bodyTransform.position.y == doctest::Approx(16.0f));
	CHECK(bodyTransform.angle == doctest::Approx(0.5f));

	physicsEngine->SetBodyAwake(body, true);
	CHECK(mock->lastBodyAwake == true);

	physicsEngine->SetBodyGravityScale(body, 0.25f);
	CHECK(mock->lastGravityScale == doctest::Approx(0.25f));

	mock->linearVelocityResult = Vector2F(3.0f, -4.0f);

	auto velocity = physicsEngine->GetLinearVelocity(body);
	CHECK(velocity.x == doctest::Approx(3.0f));
	CHECK(velocity.y == doctest::Approx(-4.0f));

	physicsEngine->SetLinearVelocity(body, Vector2F(5.0f, 6.0f));
	CHECK(mock->lastLinearVelocity.x == doctest::Approx(5.0f));
	CHECK(mock->lastLinearVelocity.y == doctest::Approx(6.0f));

	physicsEngine->ApplyLinearImpulseToCenter(body, Vector2F(7.0f, 8.0f));
	CHECK(mock->lastImpulse.x == doctest::Approx(7.0f));
	CHECK(mock->lastImpulse.y == doctest::Approx(8.0f));

	physicsEngine->ApplyForceToCenter(body, Vector2F(9.0f, 10.0f));
	CHECK(mock->lastForce.x == doctest::Approx(9.0f));
	CHECK(mock->lastForce.y == doctest::Approx(10.0f));
}


TEST_CASE("GetCollisionMasks exposes the injected masks")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	CHECK(physicsEngine->GetCollisionMasks().GetMaskFlagByName("DEFAULT") == 1);
	CHECK(physicsEngine->GetCollisionMasks().GetMaskFlagByName("AUDIO") == 2);
	CHECK(physicsEngine->GetCollisionMasks().GetMaskFlagByName("ALL") == 65535);
	CHECK(physicsEngine->GetCollisionMasks().GetMaskFlagByName("MISSING") == 0);
}


TEST_CASE("GetPhysicsConfig exposes the injected configuration")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	CHECK(physicsEngine->GetPhysicsConfig().velocityIterations == 6);
	CHECK(physicsEngine->GetPhysicsConfig().positionIterations == 2);
	CHECK(physicsEngine->GetPhysicsConfig().pixelPerMeter == doctest::Approx(40.0f));
}


TEST_CASE("EndDraw delegates debug drawing to the backend")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	ICoreSystem* system = physicsEngine.get();

	system->EndDraw();

	CHECK(mock->debugDrawCount == 1);
	CHECK(mock->lastDebugDrawTarget != nullptr);
}


TEST_CASE("SetDebugDrawEnabled false suppresses debug drawing")
{
	MockPhysicsBackend* mock = nullptr;
	auto physicsEngine = MakePhysicsEngine(mock);

	physicsEngine->SetDebugDrawEnabled(false);

	ICoreSystem* system = physicsEngine.get();

	system->EndDraw();

	CHECK(mock->debugDrawCount == 0);
}


TEST_SUITE_END();