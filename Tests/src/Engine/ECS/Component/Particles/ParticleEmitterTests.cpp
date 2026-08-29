#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Math/MathUtils.h"
#include "Constants/Rendering/DefaultSortOrders.h"
#include "Data/Components/Particles/ParticleEmitterProperties.h"
#include "Engine/ECS/Component/Particles/ParticleEmitter.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <cmath>
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// Wires a TestGameObject with a ParticleEmitter against a mock-backed TextureManager,
	// mirroring what Scene normally does via Init() and Start().
	struct ParticleEmitterFixture
	{
		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* backend = nullptr;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;

		ComponentHandle<ParticleEmitter> emitter;


		ParticleEmitterFixture(ParticleEmitterProperties properties, bool wireCoreContext = true)
		{
			auto mock = std::make_unique<MockTextureBackend>();
			backend = mock.get();
			textureManager = std::make_unique<TextureManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();

			if (wireCoreContext)
			{
				scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });
			}

			owner = scene->Create<TestGameObject>();

			emitter = owner->AddComponent<ParticleEmitter>(std::move(properties));
		}

		void Run()
		{
			emitter->Init();
			emitter->Start();
		}
	};


	// Deterministic baseline: nothing is emitted and nothing dies unless a test asks for it.
	ParticleEmitterProperties MakeProps()
	{
		return ParticleEmitterProperties
		{
			.texturePath = "particle.png",
			.duration = 1000.0f,
			.loop = false,
			.emissionRate = 0.0f,
			.burstCount = 0,
			.maxParticles = 512,
			.playOnStart = true,
			.directionAngle = 0.0f,
			.directionSpread = 0.0f,
			.startSpeedMin = 0.0f,
			.startSpeedMax = 0.0f,
			.lifetimeMin = 1000.0f,
			.lifetimeMax = 1000.0f,
			.startSizeMin = 4.0f,
			.startSizeMax = 4.0f,
			.randomSeed = 12345u
		};
	}
}


TEST_SUITE_BEGIN("ParticleEmitter");


TEST_CASE("Init throws when CoreContext has no TextureManager")
{
	auto fixture = ParticleEmitterFixture(MakeProps(), false);

	CHECK_THROWS_AS(fixture.emitter->Init(), std::invalid_argument);
}


TEST_CASE("Init loads the configured texture through the TextureManager")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.emitter->Init();

	CHECK(fixture.backend->loadCount == 1);
	CHECK(fixture.emitter->GetTexture() != 0);
}


TEST_CASE("Init reserves capacity for maxParticles so the particle buffer never reallocates")
{
	auto properties = MakeProps();
	properties.maxParticles = 64;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetParticles().capacity() >= 64);

	fixture.emitter->Burst(64);

	const auto* firstElement = fixture.emitter->GetParticles().data();

	fixture.emitter->Update(0.016f);

	CHECK(fixture.emitter->GetParticles().data() == firstElement);
}


TEST_CASE("Start emits the configured burst immediately when playOnStart is true")
{
	auto properties = MakeProps();
	properties.burstCount = 10;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetAliveCount() == 10);
	CHECK(fixture.emitter->IsPlaying());
}


TEST_CASE("Start emits nothing when playOnStart is false")
{
	auto properties = MakeProps();
	properties.burstCount = 10;
	properties.playOnStart = false;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetAliveCount() == 0);
	CHECK_FALSE(fixture.emitter->IsPlaying());
}


TEST_CASE("Update emits particles at the configured emission rate")
{
	auto properties = MakeProps();
	properties.emissionRate = 10.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(1.0f);

	CHECK(fixture.emitter->GetAliveCount() == 10);
}


TEST_CASE("Update carries the fractional emission remainder across frames instead of discarding it")
{
	auto properties = MakeProps();
	properties.emissionRate = 20.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	// A single 60 Hz frame is worth 0.33 particles: truncating it would emit nothing, ever.
	fixture.emitter->Update(1.0f / 60.0f);

	CHECK(fixture.emitter->GetAliveCount() == 0);

	for (auto frame = 0; frame < 59; ++frame)
	{
		fixture.emitter->Update(1.0f / 60.0f);
	}

	CHECK(fixture.emitter->GetAliveCount() >= 19);
	CHECK(fixture.emitter->GetAliveCount() <= 20);
}


TEST_CASE("Update emits nothing when the emission rate is zero")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.Run();
	fixture.emitter->Update(5.0f);

	CHECK(fixture.emitter->GetAliveCount() == 0);
}


TEST_CASE("Update never lets the live particle count exceed maxParticles")
{
	auto properties = MakeProps();
	properties.emissionRate = 1000.0f;
	properties.maxParticles = 5;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(1.0f);
	fixture.emitter->Update(1.0f);

	CHECK(fixture.emitter->GetAliveCount() == 5);
}


TEST_CASE("Update drops the excess rather than recycling the oldest particle when at capacity")
{
	auto properties = MakeProps();
	properties.maxParticles = 2;
	properties.burstCount = 2;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);
	fixture.emitter->Burst(10);

	CHECK(fixture.emitter->GetAliveCount() == 2);

	// Both survivors are the originals, so no age was reset by a recycled spawn.
	auto allAgesUnchanged = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		if (particle.age != doctest::Approx(0.5f))
		{
			allAgesUnchanged = false;
		}
	}

	CHECK(allAgesUnchanged);
}


TEST_CASE("Update removes a particle once its age reaches its lifetime")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.lifetimeMin = 0.5f;
	properties.lifetimeMax = 0.5f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.4f);

	CHECK(fixture.emitter->GetAliveCount() == 1);

	fixture.emitter->Update(0.2f);

	CHECK(fixture.emitter->GetAliveCount() == 0);
}


TEST_CASE("Update applies gravity to particle velocity")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.gravity = Vector2F(0.0f, 100.0f);

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetParticles()[0].velocity.y == doctest::Approx(50.0f));
	CHECK(fixture.emitter->GetParticles()[0].position.y == doctest::Approx(25.0f));
}


TEST_CASE("Update applies damping so particle speed decreases over time")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSpeedMin = 100.0f;
	properties.startSpeedMax = 100.0f;
	properties.damping = 1.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetParticles()[0].velocity.x == doctest::Approx(100.0f));

	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetParticles()[0].velocity.x == doctest::Approx(50.0f));
}


TEST_CASE("Update advances particle rotation by its angular velocity")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.angularVelocityMin = 90.0f;
	properties.angularVelocityMax = 90.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetParticles()[0].rotation == doctest::Approx(45.0f));
}


TEST_CASE("A non-looping emitter stops emitting once its duration elapses but keeps simulating live particles")
{
	auto properties = MakeProps();
	properties.emissionRate = 10.0f;
	properties.duration = 0.4f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 5);

	fixture.emitter->Update(1.0f);

	CHECK(fixture.emitter->GetAliveCount() == 5);
	CHECK(fixture.emitter->IsPlaying());
}


TEST_CASE("A looping emitter keeps emitting past its duration")
{
	auto properties = MakeProps();
	properties.emissionRate = 8.0f;
	properties.duration = 0.25f;
	properties.loop = true;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	const auto afterFirstCycle = fixture.emitter->GetAliveCount();

	fixture.emitter->Update(0.5f);

	CHECK(afterFirstCycle > 0);
	CHECK(fixture.emitter->GetAliveCount() > afterFirstCycle);
}


TEST_CASE("A looping emitter re-fires its burst on every cycle")
{
	auto properties = MakeProps();
	properties.burstCount = 3;
	properties.duration = 0.5f;
	properties.loop = true;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetAliveCount() == 3);

	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 6);

	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 9);
}


TEST_CASE("An emitter with zero duration and no looping emits its burst once and immediately stops emitting")
{
	auto properties = MakeProps();
	properties.burstCount = 5;
	properties.emissionRate = 100.0f;
	properties.duration = 0.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetAliveCount() == 5);

	fixture.emitter->Update(1.0f);

	CHECK(fixture.emitter->GetAliveCount() == 5);
}


TEST_CASE("IsFinished stays false while particles are still alive after emission ends")
{
	auto properties = MakeProps();
	properties.burstCount = 5;
	properties.duration = 0.0f;
	properties.lifetimeMin = 1.0f;
	properties.lifetimeMax = 1.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 5);
	CHECK_FALSE(fixture.emitter->IsFinished());
}


TEST_CASE("IsFinished becomes true once a one-shot emitter has stopped and all its particles have died")
{
	auto properties = MakeProps();
	properties.burstCount = 5;
	properties.duration = 0.0f;
	properties.lifetimeMin = 0.2f;
	properties.lifetimeMax = 0.2f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK_FALSE(fixture.emitter->IsFinished());

	fixture.emitter->Update(0.3f);

	CHECK(fixture.emitter->GetAliveCount() == 0);
	CHECK(fixture.emitter->IsFinished());
}


TEST_CASE("IsFinished never becomes true for a looping emitter")
{
	auto properties = MakeProps();
	properties.emissionRate = 10.0f;
	properties.duration = 0.2f;
	properties.loop = true;
	properties.lifetimeMin = 0.1f;
	properties.lifetimeMax = 0.1f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	auto neverFinished = true;

	for (auto frame = 0; frame < 120; ++frame)
	{
		fixture.emitter->Update(0.05f);

		if (fixture.emitter->IsFinished())
		{
			neverFinished = false;
		}
	}

	CHECK(neverFinished);
}


TEST_CASE("Burst emits the requested number of particles immediately")
{
	auto properties = MakeProps();
	properties.playOnStart = false;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Burst(7);

	CHECK(fixture.emitter->GetAliveCount() == 7);
	CHECK(fixture.emitter->IsPlaying());
}


TEST_CASE("Burst is clamped by maxParticles")
{
	auto properties = MakeProps();
	properties.maxParticles = 4;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Burst(50);

	CHECK(fixture.emitter->GetAliveCount() == 4);
}


TEST_CASE("Stop halts emission but lets existing particles finish their lifetime")
{
	auto properties = MakeProps();
	properties.emissionRate = 10.0f;
	properties.lifetimeMin = 2.0f;
	properties.lifetimeMax = 2.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 5);

	fixture.emitter->Stop();
	fixture.emitter->Update(0.5f);

	CHECK(fixture.emitter->GetAliveCount() == 5);
	CHECK(fixture.emitter->IsPlaying());

	fixture.emitter->Update(1.5f);

	CHECK(fixture.emitter->GetAliveCount() == 0);
	CHECK(fixture.emitter->IsFinished());
}


TEST_CASE("Stop with clearParticles removes every live particle immediately")
{
	auto properties = MakeProps();
	properties.burstCount = 8;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	CHECK(fixture.emitter->GetAliveCount() == 8);

	fixture.emitter->Stop(true);

	CHECK(fixture.emitter->GetAliveCount() == 0);
	CHECK_FALSE(fixture.emitter->IsPlaying());
}


TEST_CASE("Play restarts a finished one-shot emitter")
{
	auto properties = MakeProps();
	properties.burstCount = 5;
	properties.duration = 0.0f;
	properties.lifetimeMin = 0.2f;
	properties.lifetimeMax = 0.2f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Update(0.3f);

	CHECK(fixture.emitter->IsFinished());

	fixture.emitter->Play();

	CHECK(fixture.emitter->GetAliveCount() == 5);
	CHECK(fixture.emitter->IsPlaying());
	CHECK_FALSE(fixture.emitter->IsFinished());
}


TEST_CASE("Two emitters sharing a random seed produce identical particle state")
{
	auto properties = MakeProps();
	properties.randomSeed = 99u;
	properties.emissionRate = 30.0f;
	properties.shape = EmitterShape::CIRCLE;
	properties.shapeSize = Vector2F(6.0f, 0.0f);
	properties.directionSpread = 90.0f;
	properties.startSpeedMin = 10.0f;
	properties.startSpeedMax = 50.0f;
	properties.lifetimeMin = 0.4f;
	properties.lifetimeMax = 0.9f;

	auto first = ParticleEmitterFixture(properties);
	auto second = ParticleEmitterFixture(properties);

	first.Run();
	second.Run();

	for (auto frame = 0; frame < 30; ++frame)
	{
		first.emitter->Update(0.02f);
		second.emitter->Update(0.02f);
	}

	const auto& a = first.emitter->GetParticles();
	const auto& b = second.emitter->GetParticles();

	REQUIRE(a.size() == b.size());
	REQUIRE(a.size() > 0);

	auto allParticlesMatch = true;

	for (size_t i = 0; i < a.size(); ++i)
	{
		if (a[i].position.x != doctest::Approx(b[i].position.x)
			|| a[i].position.y != doctest::Approx(b[i].position.y)
			|| a[i].velocity.x != doctest::Approx(b[i].velocity.x)
			|| a[i].lifetime != doctest::Approx(b[i].lifetime)
			|| a[i].startSize != doctest::Approx(b[i].startSize))
		{
			allParticlesMatch = false;
		}
	}

	CHECK(allParticlesMatch);
}


TEST_CASE("Particles spawned from a circle shape start within the configured radius")
{
	auto properties = MakeProps();
	properties.burstCount = 300;
	properties.shape = EmitterShape::CIRCLE;
	properties.shapeSize = Vector2F(10.0f, 0.0f);

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	REQUIRE(fixture.emitter->GetAliveCount() == 300);

	auto allWithinRadius = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		const auto distance = particle.position.Magnitude();

		if (distance > 10.001f)
		{
			allWithinRadius = false;
		}
	}

	CHECK(allWithinRadius);
}


TEST_CASE("Particles spawned from a box shape start within the configured half extents")
{
	auto properties = MakeProps();
	properties.burstCount = 300;
	properties.shape = EmitterShape::BOX;
	properties.shapeSize = Vector2F(8.0f, 3.0f);

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	REQUIRE(fixture.emitter->GetAliveCount() == 300);

	auto allWithinExtents = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		if (std::abs(particle.position.x) > 8.001f || std::abs(particle.position.y) > 3.001f)
		{
			allWithinExtents = false;
		}
	}

	CHECK(allWithinExtents);
}


TEST_CASE("Radial velocity sends particles away from the emitter centre")
{
	auto properties = MakeProps();
	properties.burstCount = 200;
	properties.shape = EmitterShape::CIRCLE;
	properties.shapeSize = Vector2F(5.0f, 0.0f);
	properties.radialVelocity = true;
	properties.startSpeedMin = 10.0f;
	properties.startSpeedMax = 10.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	auto allOutward = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		const auto outwardDot = particle.position.Dot(particle.velocity);

		if (outwardDot <= 0.0f)
		{
			allOutward = false;
		}
	}

	CHECK(allOutward);
}


TEST_CASE("The direction spread keeps initial velocity within the configured cone")
{
	auto properties = MakeProps();
	properties.burstCount = 200;
	properties.directionAngle = 0.0f;
	properties.directionSpread = 20.0f;
	properties.startSpeedMin = 10.0f;
	properties.startSpeedMax = 10.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	auto allWithinCone = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		const auto degrees = MathUtils::ToDegrees(std::atan2(particle.velocity.y, particle.velocity.x));

		if (degrees < -10.001f || degrees > 10.001f)
		{
			allWithinCone = false;
		}
	}

	CHECK(allWithinCone);
}


TEST_CASE("World-space particles keep their position when the emitter transform moves")
{
	auto properties = MakeProps();
	properties.burstCount = 3;
	properties.simulationSpace = SimulationSpace::WORLD;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(100.0f, 0.0f));

	fixture.Run();

	REQUIRE(fixture.emitter->GetAliveCount() == 3);

	auto allAtOriginalXBeforeMove = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		if (particle.position.x != doctest::Approx(100.0f))
		{
			allAtOriginalXBeforeMove = false;
		}
	}

	CHECK(allAtOriginalXBeforeMove);

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(200.0f, 0.0f));
	fixture.emitter->Update(0.1f);

	auto allAtOriginalXAfterMove = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		if (particle.position.x != doctest::Approx(100.0f))
		{
			allAtOriginalXAfterMove = false;
		}
	}

	CHECK(allAtOriginalXAfterMove);
}


TEST_CASE("Local-space particles are stored relative to the emitter transform")
{
	auto properties = MakeProps();
	properties.burstCount = 3;
	properties.simulationSpace = SimulationSpace::LOCAL;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(100.0f, 25.0f));

	fixture.Run();

	REQUIRE(fixture.emitter->GetAliveCount() == 3);

	auto allAtLocalOrigin = true;

	for (const auto& particle : fixture.emitter->GetParticles())
	{
		if (particle.position.x != doctest::Approx(0.0f) || particle.position.y != doctest::Approx(0.0f))
		{
			allAtLocalOrigin = false;
		}
	}

	CHECK(allAtLocalOrigin);
}


TEST_CASE("Draw produces exactly one sprite entry per live particle")
{
	auto properties = MakeProps();
	properties.burstCount = 12;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch.size() == 12);
}


TEST_CASE("Draw produces a centre-anchored destination rect matching the SpriteRenderer convention")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSizeMin = 8.0f;
	properties.startSizeMax = 8.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	const auto& sprite = fixture.emitter->GetBatchData().spriteBatch[0];

	REQUIRE(sprite.destRect.has_value());

	CHECK(sprite.destRect->x == doctest::Approx(-4.0f));
	CHECK(sprite.destRect->y == doctest::Approx(-4.0f));
	CHECK(sprite.destRect->w == doctest::Approx(8.0f));
	CHECK(sprite.destRect->h == doctest::Approx(8.0f));
}


TEST_CASE("Draw interpolates particle size from startSize toward startSize scaled by endSizeMultiplier")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSizeMin = 10.0f;
	properties.startSizeMax = 10.0f;
	properties.endSizeMultiplier = 0.0f;
	properties.lifetimeMin = 1.0f;
	properties.lifetimeMax = 1.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->w == doctest::Approx(10.0f));

	fixture.emitter->Update(0.5f);
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->w == doctest::Approx(5.0f));
}


TEST_CASE("Draw interpolates particle colour and alpha from startColor to endColor across the particle lifetime")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.lifetimeMin = 1.0f;
	properties.lifetimeMax = 1.0f;
	properties.startColor = Color{ .r = 255, .g = 0, .b = 0, .a = 255 };
	properties.endColor = Color{ .r = 0, .g = 0, .b = 255, .a = 0 };

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].colorMod == properties.startColor);

	fixture.emitter->Update(0.5f);
	fixture.emitter->Draw();

	const auto midColor = fixture.emitter->GetBatchData().spriteBatch[0].colorMod;

	CHECK(midColor.r == 128);
	CHECK(midColor.b == 128);
	CHECK(midColor.a == 128);
}


TEST_CASE("Draw ignores the transform world scale so particle size stays in configured pixels")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSizeMin = 8.0f;
	properties.startSizeMax = 8.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.owner->GetTransform()->SetWorldScale(Vector2F(3.0f, 3.0f));

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->w == doctest::Approx(8.0f));
	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->h == doctest::Approx(8.0f));
}


TEST_CASE("Draw offsets local-space particles by the emitter current world position")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSizeMin = 8.0f;
	properties.startSizeMax = 8.0f;
	properties.simulationSpace = SimulationSpace::LOCAL;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(100.0f, 25.0f));

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->x == doctest::Approx(96.0f));
	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->y == doctest::Approx(21.0f));

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(500.0f, 25.0f));
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->x == doctest::Approx(496.0f));
}


TEST_CASE("Draw leaves world-space particles unaffected by the emitter world position")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startSizeMin = 8.0f;
	properties.startSizeMax = 8.0f;
	properties.simulationSpace = SimulationSpace::WORLD;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(100.0f, 0.0f));

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->x == doctest::Approx(96.0f));

	fixture.owner->GetTransform()->SetWorldPosition(Vector2F(500.0f, 0.0f));
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].destRect->x == doctest::Approx(96.0f));
}


TEST_CASE("Draw sets rotation from the particle own rotation when velocity alignment is disabled")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.startRotationMin = 30.0f;
	properties.startRotationMax = 30.0f;
	properties.alignRotationToVelocity = false;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].rotation == doctest::Approx(30.0f));
}


TEST_CASE("Draw aligns rotation to the velocity direction when velocity alignment is enabled")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.directionAngle = 90.0f;
	properties.directionSpread = 0.0f;
	properties.startSpeedMin = 10.0f;
	properties.startSpeedMax = 10.0f;
	properties.startRotationMin = 30.0f;
	properties.startRotationMax = 30.0f;
	properties.alignRotationToVelocity = true;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	CHECK(fixture.emitter->GetBatchData().spriteBatch[0].rotation == doctest::Approx(90.0f).epsilon(0.01));
}


TEST_CASE("Draw reuses the sprite batch buffer so its capacity and data pointer are unchanged between frames")
{
	auto properties = MakeProps();
	properties.burstCount = 10;
	properties.maxParticles = 64;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	const auto* firstElement = fixture.emitter->GetBatchData().spriteBatch.data();
	const auto capacity = fixture.emitter->GetBatchData().spriteBatch.capacity();

	for (auto frame = 0; frame < 10; ++frame)
	{
		fixture.emitter->Update(0.016f);
		fixture.emitter->Draw();
	}

	CHECK(fixture.emitter->GetBatchData().spriteBatch.data() == firstElement);
	CHECK(fixture.emitter->GetBatchData().spriteBatch.capacity() == capacity);
}


TEST_CASE("Draw clears the batch and submits nothing when no particles are alive")
{
	auto properties = MakeProps();
	properties.burstCount = 6;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	REQUIRE(fixture.emitter->GetBatchData().spriteBatch.size() == 6);

	fixture.emitter->Stop(true);

	CHECK_NOTHROW(fixture.emitter->Draw());
	CHECK(fixture.emitter->GetBatchData().spriteBatch.empty());
}


TEST_CASE("Draw does not throw when the emitter texture failed to load")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.backend->failNextLoad = true;

	fixture.Run();
	fixture.emitter->Burst(4);

	CHECK(fixture.emitter->GetTexture() == 0);
	CHECK_NOTHROW(fixture.emitter->Draw());
}


TEST_CASE("Draw forwards the configured source rect to every sprite in the batch")
{
	auto properties = MakeProps();
	properties.burstCount = 5;
	properties.sourceRect = RectI{ .x = 1, .y = 2, .w = 3, .h = 4 };

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	auto allHaveExpectedSourceRect = true;

	for (const auto& sprite : fixture.emitter->GetBatchData().spriteBatch)
	{
		if (!sprite.srcRect.has_value() || sprite.srcRect.value() != properties.sourceRect.value())
		{
			allHaveExpectedSourceRect = false;
		}
	}

	CHECK(allHaveExpectedSourceRect);
}


TEST_CASE("The emitter uses the particle sort order by default and honours a configured override")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.Run();

	CHECK(fixture.emitter->GetSortOrder() == DF2D::Constants::DefaultSortOrders::PARTICLE_RENDERER);

	auto properties = MakeProps();
	properties.sortOrder = -7;

	auto behind = ParticleEmitterFixture(properties);

	behind.Run();

	CHECK(behind.emitter->GetSortOrder() == -7);
}


TEST_CASE("Draw tags every sprite in the batch with the emitter configured blend mode")
{
	auto properties = MakeProps();
	properties.burstCount = 6;
	properties.blendMode = BlendMode::ADDITIVE;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();
	fixture.emitter->Draw();

	REQUIRE(fixture.emitter->GetBatchData().spriteBatch.size() == 6);

	auto allAdditive = true;

	for (const auto& sprite : fixture.emitter->GetBatchData().spriteBatch)
	{
		if (sprite.blendMode != BlendMode::ADDITIVE)
		{
			allAdditive = false;
		}
	}

	CHECK(allAdditive);
}


TEST_CASE("SetProperties reseeds the generator when the seed value changes")
{
	auto properties = MakeProps();
	properties.burstCount = 4;
	properties.randomSeed = 1u;
	properties.startSpeedMin = 0.0f;
	properties.startSpeedMax = 100.0f;

	auto reference = ParticleEmitterFixture(properties);
	reference.Run();

	// An emitter constructed with seed 7 is the ground truth for what seed 7 produces.
	auto seven = properties;
	seven.randomSeed = 7u;

	auto expected = ParticleEmitterFixture(seven);
	expected.Run();

	auto switched = ParticleEmitterFixture(properties);
	switched.Run();
	switched.emitter->Stop(true);
	switched.emitter->SetProperties(seven);
	switched.emitter->Play();

	REQUIRE(switched.emitter->GetAliveCount() == expected.emitter->GetAliveCount());

	auto allVelocitiesMatch = true;

	for (size_t i = 0; i < expected.emitter->GetAliveCount(); ++i)
	{
		if (switched.emitter->GetParticles()[i].velocity.x != doctest::Approx(expected.emitter->GetParticles()[i].velocity.x))
		{
			allVelocitiesMatch = false;
		}
	}

	CHECK(allVelocitiesMatch);
}


TEST_CASE("SetProperties leaves the running sequence alone when the seed is unchanged")
{
	auto properties = MakeProps();
	properties.burstCount = 1;
	properties.maxParticles = 64;
	properties.startSpeedMin = 0.0f;
	properties.startSpeedMax = 100.0f;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	// Mimics a debug slider: same seed, one tweaked field, applied every frame.
	auto distinctSpeeds = 0;

	auto previousSpeed = fixture.emitter->GetParticles()[0].velocity.x;

	for (auto frame = 0; frame < 20; ++frame)
	{
		properties.damping = static_cast<float>(frame) * 0.01f;

		fixture.emitter->SetProperties(properties);
		fixture.emitter->Burst(1);

		const auto speed = fixture.emitter->GetParticles().back().velocity.x;

		if (speed != doctest::Approx(previousSpeed))
		{ 
			++distinctSpeeds;
		}

		previousSpeed = speed;
	}

	// A reseed every call would replay one identical spawn forever.
	CHECK(distinctSpeeds > 10);
}


TEST_CASE("SetProperties reloads the texture when the path changes")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.Run();

	const auto originalTexture = fixture.emitter->GetTexture();

	REQUIRE(fixture.backend->loadCount == 1);

	auto properties = MakeProps();
	properties.texturePath = "other_particle.png";

	fixture.emitter->SetProperties(properties);

	CHECK(fixture.backend->loadCount == 2);
	CHECK(fixture.backend->lastLoadedFile.find("other_particle.png") != std::string::npos);
	CHECK(fixture.emitter->GetTexture() != originalTexture);
}


TEST_CASE("SetProperties does not reload the texture when the path is unchanged")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.Run();

	fixture.emitter->SetProperties(MakeProps());
	fixture.emitter->SetProperties(MakeProps());

	CHECK(fixture.backend->loadCount == 1);
}


TEST_CASE("SetProperties applies a new sort order")
{
	auto fixture = ParticleEmitterFixture(MakeProps());

	fixture.Run();

	auto properties = MakeProps();
	properties.sortOrder = -12;

	fixture.emitter->SetProperties(properties);

	CHECK(fixture.emitter->GetSortOrder() == -12);
}


TEST_CASE("SetProperties re-reserves the particle buffer when maxParticles grows")
{
	auto properties = MakeProps();
	properties.maxParticles = 8;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	properties.maxParticles = 256;

	fixture.emitter->SetProperties(properties);

	REQUIRE(fixture.emitter->GetParticles().capacity() >= 256);

	// Filling to the new cap must not reallocate, which is what the reserve protects.
	const auto* firstElement = fixture.emitter->GetParticles().data();

	fixture.emitter->Burst(256);

	CHECK(fixture.emitter->GetAliveCount() == 256);
	CHECK(fixture.emitter->GetParticles().data() == firstElement);
}


TEST_CASE("SetProperties trims live particles when maxParticles shrinks")
{
	auto properties = MakeProps();
	properties.maxParticles = 32;
	properties.burstCount = 32;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	REQUIRE(fixture.emitter->GetAliveCount() == 32);

	properties.maxParticles = 5;

	fixture.emitter->SetProperties(properties);

	CHECK(fixture.emitter->GetAliveCount() == 5);
}


TEST_CASE("SetProperties clamps a negative maxParticles to zero instead of stalling emission")
{
	auto properties = MakeProps();
	properties.burstCount = 4;

	auto fixture = ParticleEmitterFixture(properties);

	fixture.Run();

	properties.maxParticles = -10;

	fixture.emitter->SetProperties(properties);

	CHECK(fixture.emitter->GetProperties().maxParticles == 0);
	CHECK(fixture.emitter->GetAliveCount() == 0);

	// Restoring a sane cap must let emission resume.
	properties.maxParticles = 16;

	fixture.emitter->SetProperties(properties);
	fixture.emitter->Burst(16);

	CHECK(fixture.emitter->GetAliveCount() == 16);
}


TEST_CASE("SetProperties called before Init leaves the texture load to Init")
{
	auto properties = MakeProps();
	properties.texturePath = "deferred.png";

	auto fixture = ParticleEmitterFixture(MakeProps());

	CHECK_NOTHROW(fixture.emitter->SetProperties(properties));
	CHECK(fixture.backend->loadCount == 0);

	fixture.Run();

	CHECK(fixture.backend->loadCount == 1);
	CHECK(fixture.emitter->GetTexture() != 0);
}


TEST_SUITE_END();