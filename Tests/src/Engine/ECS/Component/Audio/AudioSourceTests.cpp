#include "Core/Context/Systems/Audio/AudioManager.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Audio/AudioListener.h"
#include "Engine/ECS/Component/Audio/AudioSource.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Context/Systems/Audio/MockAudioBackend.h"
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

	// Wires an AudioManager + PhysicsEngine2D against mock backends behind a fake
	// scene, mirroring what SystemInitializer/Scene provide via CoreContext.
	struct AudioFixture
	{
		std::unique_ptr<AudioManager> audioManager;

		MockAudioBackend* audioBackend;

		std::unique_ptr<PhysicsEngine2D> physicsEngine;

		MockPhysicsBackend* physicsBackend;

		std::shared_ptr<FakeSceneHandleProvider> scene;


		AudioFixture()
		{
			auto audioMock = std::make_unique<MockAudioBackend>();
			audioBackend = audioMock.get();
			audioManager = std::make_unique<AudioManager>(AudioConfig{}, std::move(audioMock));

			auto physicsMock = std::make_unique<MockPhysicsBackend>();
			physicsBackend = physicsMock.get();
			physicsEngine = std::make_unique<PhysicsEngine2D>(MakePhysicsConfig(), MakeCollisionMasks(), std::move(physicsMock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .audioManager = audioManager.get(), .physicsEngine = physicsEngine.get() });
		}
	};
}


TEST_SUITE_BEGIN("AudioSource");


TEST_CASE("Parameterized constructor defers loading until Init")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();

	ComponentHandle<AudioSource> source;

	CHECK_NOTHROW(source = owner->AddComponent<AudioSource>("theme.ogg", /*isMusic*/ true, 1.0f));

	// audioManager is only wired in Init(); the constructor must not have touched it
	CHECK(fixture.audioBackend->loadMusicCount == 0);

	CHECK_NOTHROW(source->Init());

	CHECK(fixture.audioBackend->loadMusicCount == 1);
}

TEST_CASE("Init loads a pending sfx source and Play routes through the AudioManager")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>("jump.wav", /*isMusic*/ false, 0.5f);

	source->Init();

	CHECK(fixture.audioBackend->loadSFXCount == 1);

	source->Play();

	CHECK(fixture.audioBackend->playChannelCount == 1);
	CHECK(fixture.audioBackend->lastPlayedSFX == 1);
}

TEST_CASE("IsPlaying reflects channel state, and LateUpdate clears a channel once it actually finishes")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>("jump.wav", /*isMusic*/ false, 1.0f);
	source->Init();

	source->Play();

	CHECK(source->IsPlaying());

	// Clip is still playing: LateUpdate must not touch the channel
	source->LateUpdate(0.016f);
	CHECK(source->IsPlaying());

	// SDL reports the channel naturally finished
	fixture.audioBackend->isChannelPlayingResult = false;
	source->LateUpdate(0.016f);

	CHECK_FALSE(source->IsPlaying());
}

TEST_CASE("Stop does not halt a channel that already finished naturally")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>("jump.wav", /*isMusic*/ false, 1.0f);
	source->Init();

	source->Play();

	// The clip finishes naturally; SDL_mixer is now free to hand the channel to someone else
	fixture.audioBackend->isChannelPlayingResult = false;
	source->LateUpdate(0.016f);

	fixture.audioBackend->lastStoppedChannel = -99; // sentinel

	source->Stop();

	// A possibly-reassigned channel must never be halted on behalf of a finished clip
	CHECK(fixture.audioBackend->lastStoppedChannel == -99);
}

TEST_CASE("Stop releases ownership of the channel (regression: a second Stop must not re-halt a possibly-reassigned channel)")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>("jump.wav", /*isMusic*/ false, 1.0f);
	source->Init();

	source->Play();

	CHECK(source->IsPlaying());

	source->Stop();

	// Stop() must give up the channel immediately, not just silence it
	CHECK_FALSE(source->IsPlaying());

	fixture.audioBackend->lastStoppedChannel = -99; // sentinel

	source->Stop();

	// A second Stop() has nothing of ours left to halt
	CHECK(fixture.audioBackend->lastStoppedChannel == -99);
}

TEST_CASE("Default constructor does not eagerly load any audio")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();

	source->Init();

	CHECK(fixture.audioBackend->loadMusicCount == 0);
	CHECK(fixture.audioBackend->loadSFXCount == 0);
}

TEST_CASE("Update builds a kinematic sensor circle sized to the max reaching distance, filtered to AUDIO")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();
	source->Init();

	source->Update(0.016f);

	REQUIRE(fixture.physicsBackend->createdBodies.size() == 1);
	CHECK(fixture.physicsBackend->lastBodyDefinition.type == BodyType2D::Kinematic);

	REQUIRE(fixture.physicsBackend->createdFixtures.size() == 1);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.isSensor);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.filter.categoryBits == 2);
	CHECK(fixture.physicsBackend->lastPhysicsMaterial.filter.maskBits == 2);

	auto* circle = std::get_if<CircleShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);

	REQUIRE(circle != nullptr);
	CHECK(circle->radius == doctest::Approx(9999.0f));
}

TEST_CASE("SetMaxDistance rebuilds the fixture with the new radius")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();
	source->Init();
	source->Update(0.016f);

	source->SetMaxDistance(200.0f);
	source->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == 2);
	CHECK(fixture.physicsBackend->destroyedFixtures.size() == 1);

	auto* circle = std::get_if<CircleShapeDefinition2D>(&fixture.physicsBackend->lastPhysicsMaterial.shape);

	REQUIRE(circle != nullptr);
	CHECK(circle->radius == doctest::Approx(200.0f));
}

TEST_CASE("SetMinDistance does not rebuild the fixture")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();
	source->Init();
	source->Update(0.016f);

	auto fixtureCountBefore = fixture.physicsBackend->createdFixtures.size();

	source->SetMinDistance(50.0f);
	source->Update(0.016f);

	CHECK(fixture.physicsBackend->createdFixtures.size() == fixtureCountBefore);
}

TEST_CASE("LateUpdate moves the body and wakes it when the transform changes")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();
	source->Init();
	source->Update(0.016f);

	owner->GetTransform()->SetWorldPosition(Vector2F(100.0f, 50.0f));

	source->LateUpdate(0.016f);

	CHECK(fixture.physicsBackend->lastBodyPosition.x == doctest::Approx(100.0f));
	CHECK(fixture.physicsBackend->lastBodyPosition.y == doctest::Approx(50.0f));
	CHECK(fixture.physicsBackend->lastBodyAwake == true);
}

TEST_CASE("Exiting contact with a non-tracked listener leaves attenuation active")
{
	AudioFixture fixture;

	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>("jump.wav", /*isMusic*/ false, 1.0f);
	source->Init();

	auto listenerAOwner = fixture.scene->Create<TestGameObject>();
	auto listenerA = listenerAOwner->AddComponent<AudioListener>();
	listenerA->Init();

	auto listenerBOwner = fixture.scene->Create<TestGameObject>();
	auto listenerB = listenerBOwner->AddComponent<AudioListener>();
	listenerB->Init();

	source->Play();

	auto enterA = CollisionInfo{ .otherGameObject = listenerAOwner };
	source->InvokeCollisionEnter(enterA);

	// B enters after A; B becomes the tracked listener
	auto enterB = CollisionInfo{ .otherGameObject = listenerBOwner };
	source->InvokeCollisionEnter(enterB);

	// A leaves contact even though B (the tracked listener) is still in range
	auto exitA = CollisionInfo{ .otherGameObject = listenerAOwner };
	source->InvokeCollisionExit(exitA);

	fixture.audioBackend->lastSFXVolume = -1.0f; // sentinel

	source->LateUpdate(0.016f);

	// Attenuation still ran against the still-in-range listener B
	CHECK(fixture.audioBackend->lastSFXVolume != doctest::Approx(-1.0f));

	// B (the actually tracked listener) now leaves contact
	auto exitB = CollisionInfo{ .otherGameObject = listenerBOwner };
	source->InvokeCollisionExit(exitB);

	fixture.audioBackend->lastSFXVolume = -1.0f;

	source->LateUpdate(0.016f);

	// No listener tracked anymore, so attenuation no longer runs
	CHECK(fixture.audioBackend->lastSFXVolume == doctest::Approx(-1.0f));
}

TEST_CASE("Removing the component destroys its physics body")
{
	AudioFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto source = owner->AddComponent<AudioSource>();
	source->Init();
	source->Update(0.016f);

	owner->RemoveComponent(source);

	CHECK(fixture.physicsBackend->destroyedBodies.size() == 1);
}


TEST_SUITE_END();