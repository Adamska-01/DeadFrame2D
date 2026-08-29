#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Data/Components/Particles/ParticlePresets.h"
#include "Engine/ECS/Component/Particles/DestroyOnParticlesFinished.h"
#include "Engine/ECS/Component/Particles/ParticleEmitter.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
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
	// Drives a real Scene so GameObject::Destroy and ProcessPendingDestructions actually run.
	struct EffectSceneFixture
	{
		EventDispatcher eventDispatcher;

		std::unique_ptr<TextureManager> textureManager;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> effect;


		EffectSceneFixture(ParticleEmitterProperties properties)
		{
			textureManager = std::make_unique<TextureManager>(std::make_unique<MockTextureBackend>());

			scene = std::make_shared<TestScene>(&eventDispatcher);
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

			effect = scene->Spawn<TestGameObject>();
			effect->AddComponent<ParticleEmitter>(std::move(properties));
			effect->AddComponent<DestroyOnParticlesFinished>();

			scene->Init();
		}

		bool IsEffectAlive() const
		{
			return effect() != nullptr;
		}

		void Advance(int frames, float deltaTime = 1.0f / 60.0f)
		{
			for (auto frame = 0; frame < frames; ++frame)
				scene->Update(deltaTime);
		}
	};


	ParticleEmitterProperties MakeOneShotProps(float lifetime)
	{
		return ParticleEmitterProperties
		{
			.texturePath = "particle.png",
			.duration = 0.0f,
			.loop = false,
			.emissionRate = 0.0f,
			.burstCount = 8,
			.maxParticles = 8,
			.startSpeedMin = 0.0f,
			.startSpeedMax = 0.0f,
			.lifetimeMin = lifetime,
			.lifetimeMax = lifetime,
			.randomSeed = 7u
		};
	}
}


TEST_SUITE_BEGIN("DestroyOnParticlesFinished");


TEST_CASE("Init throws when the owning GameObject has no ParticleEmitter")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();

	auto owner = scene->Create<TestGameObject>();
	auto destroyer = owner->AddComponent<DestroyOnParticlesFinished>();

	CHECK_THROWS_AS(destroyer->Init(), std::invalid_argument);
}


TEST_CASE("Update leaves the owner alive while the emitter still has live particles")
{
	auto fixture = EffectSceneFixture(MakeOneShotProps(1.0f));

	fixture.Advance(10);

	CHECK(fixture.IsEffectAlive());
}


TEST_CASE("Update destroys the owner once the emitter reports finished")
{
	auto fixture = EffectSceneFixture(MakeOneShotProps(0.2f));

	REQUIRE(fixture.IsEffectAlive());

	fixture.Advance(60);

	CHECK_FALSE(fixture.IsEffectAlive());
}


TEST_CASE("Update never destroys the owner of a looping emitter")
{
	auto properties = MakeOneShotProps(0.2f);
	properties.duration = 0.5f;
	properties.loop = true;

	auto fixture = EffectSceneFixture(properties);

	fixture.Advance(240);

	CHECK(fixture.IsEffectAlive());
}


TEST_CASE("An explosion effect object destroys itself after its particles expire")
{
	auto fixture = EffectSceneFixture(ParticlePresets::Explosion("particle.png"));

	REQUIRE(fixture.IsEffectAlive());

	fixture.Advance(120);

	CHECK_FALSE(fixture.IsEffectAlive());
}


TEST_SUITE_END();