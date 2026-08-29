#pragma once
#include "Core/Math/Random.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/Particles/Particle.h"
#include "Data/Components/Particles/ParticleEmitterProperties.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <vector>


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Engine
{
	class Transform;


	/**
	 * @brief Simulates and draws a pool of lightweight particles for effects such as
	 * fire, smoke, sparks, explosions and dust.
	 *
	 * Particles are plain structs held in one contiguous buffer, never GameObjects. The
	 * buffer is reserved to maxParticles in Init and never reallocates: emission past the
	 * cap is dropped, and dead particles are removed by swapping with the last live one.
	 */
	class DF2D_API ParticleEmitter : public GameComponent
	{
		TYPE_INFO(ParticleEmitter, GameComponent);


	private:
		Data::ParticleEmitterProperties properties;

		std::vector<Data::Particle> particles;

		Data::SpriteBatchRenderData batchData;

		Data::RenderTask renderTask;

		Core::Random random;

		ComponentHandle<Transform> transform;

		Core::TextureManager* textureManager = nullptr;

		Data::TextureID texture = 0;

		float emissionAccumulator = 0.0f;

		float elapsedTime = 0.0f;

		bool isPlaying = false;

		bool isEmitting = false;

		bool hasPlayed = false;


		void EmitParticles(int count);

		void SpawnParticle(const Core::Vector2F& origin);

		void IntegrateParticles(float deltaTime);

		void BuildBatch();

		Core::Vector2F GetEmissionOrigin() const;


	public:
		ParticleEmitter(Data::ParticleEmitterProperties properties);

		virtual ~ParticleEmitter() override = default;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		/** @brief Restarts emission from the beginning and fires the configured burst. */
		void Play();

		/** @brief Halts emission. Live particles keep simulating unless clearParticles is set. */
		void Stop(bool clearParticles = false);

		/** @brief Releases particles immediately, clamped by the remaining capacity. */
		void Burst(int count);

		bool IsPlaying() const;

		/** @brief True once the emitter has played, stopped emitting, and outlived its last particle. */
		bool IsFinished() const;

		size_t GetAliveCount() const;

		int GetSortOrder() const;

		Data::TextureID GetTexture() const;

		const std::vector<Data::Particle>& GetParticles() const;

		/** @brief The sprite batch built by the last Draw call. Exposed for inspection and testing. */
		const Data::SpriteBatchRenderData& GetBatchData() const;

		const Data::ParticleEmitterProperties& GetProperties() const;

		void SetProperties(const Data::ParticleEmitterProperties& value);
	};
}