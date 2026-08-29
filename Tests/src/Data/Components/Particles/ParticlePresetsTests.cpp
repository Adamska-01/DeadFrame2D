#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Data/Components/Particles/ParticlePresets.h"
#include "Engine/ECS/Component/Particles/ParticleEmitter.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>
#include <vector>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// Owns everything an emitter needs so a preset can be driven for a few frames.
	struct PresetHarness
	{
		std::unique_ptr<TextureManager> textureManager;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;

		ComponentHandle<ParticleEmitter> emitter;


		PresetHarness(ParticleEmitterProperties properties)
		{
			textureManager = std::make_unique<TextureManager>(std::make_unique<MockTextureBackend>());

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get() });

			owner = scene->Create<TestGameObject>();

			emitter = owner->AddComponent<ParticleEmitter>(std::move(properties));

			emitter->Init();
			emitter->Start();
		}

		void Advance(int frames, float deltaTime = 1.0f / 60.0f)
		{
			for (auto frame = 0; frame < frames; ++frame)
			{
				emitter->Update(deltaTime);
			}
		}
	};


	std::vector<ParticleEmitterProperties> AllPresets()
	{
		return
		{
			ParticlePresets::Fire("particle.png"),
			ParticlePresets::Smoke("particle.png"),
			ParticlePresets::Sparks("particle.png"),
			ParticlePresets::Explosion("particle.png"),
			ParticlePresets::Dust("particle.png")
		};
	}
}


TEST_SUITE_BEGIN("ParticlePresets");


TEST_CASE("Every preset produces a configuration an emitter accepts without throwing")
{
	for (const auto& properties : AllPresets())
	{
		CHECK_NOTHROW(
		{
			auto harness = PresetHarness(properties);

			harness.Advance(30);
			harness.emitter->Draw();
		});
	}
}


TEST_CASE("Every preset produces at least one live particle within its first second")
{
	for (const auto& properties : AllPresets())
	{
		auto harness = PresetHarness(properties);

		auto sawParticles = harness.emitter->GetAliveCount() > 0;

		for (auto frame = 0; frame < 60 && !sawParticles; ++frame)
		{
			harness.emitter->Update(1.0f / 60.0f);

			sawParticles = harness.emitter->GetAliveCount() > 0;
		}

		CHECK(sawParticles);
	}
}


TEST_CASE("The fire preset loops and uses additive blending")
{
	auto properties = ParticlePresets::Fire("particle.png");

	CHECK(properties.loop);
	CHECK(properties.blendMode == BlendMode::ADDITIVE);

	auto harness = PresetHarness(properties);

	harness.Advance(180);

	CHECK(harness.emitter->GetAliveCount() > 0);
	CHECK_FALSE(harness.emitter->IsFinished());
}


TEST_CASE("The smoke preset grows its particles over their lifetime")
{
	auto properties = ParticlePresets::Smoke("particle.png");

	CHECK(properties.endSizeMultiplier > 1.0f);

	auto harness = PresetHarness(properties);

	harness.Advance(30);
	harness.emitter->Draw();

	REQUIRE(harness.emitter->GetBatchData().spriteBatch.size() > 0);

	const auto firstWidth = harness.emitter->GetBatchData().spriteBatch[0].destRect->w;
	const auto startSize = harness.emitter->GetParticles()[0].startSize;

	CHECK(firstWidth >= startSize);
}


TEST_CASE("The sparks preset applies downward gravity and aligns rotation to velocity")
{
	auto properties = ParticlePresets::Sparks("particle.png");

	CHECK(properties.gravity.y > 0.0f);
	CHECK(properties.alignRotationToVelocity);
	CHECK_FALSE(properties.loop);
}


TEST_CASE("The explosion preset is a non-looping burst that reports finished on its own")
{
	auto properties = ParticlePresets::Explosion("particle.png");

	CHECK_FALSE(properties.loop);
	CHECK(properties.burstCount > 0);

	auto harness = PresetHarness(properties);

	CHECK(harness.emitter->GetAliveCount() == properties.burstCount);
	CHECK_FALSE(harness.emitter->IsFinished());

	harness.Advance(120);

	CHECK(harness.emitter->GetAliveCount() == 0);
	CHECK(harness.emitter->IsFinished());
}


TEST_CASE("The dust preset emits from a box shape and draws behind sprites")
{
	auto properties = ParticlePresets::Dust("particle.png");

	CHECK(properties.shape == EmitterShape::BOX);
	CHECK(properties.sortOrder < DF2D::Constants::DefaultSortOrders::SPRITE_RENDERER);
}


TEST_SUITE_END();