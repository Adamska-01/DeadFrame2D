#include "Constants/MathConstants.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Math/MathUtils.h"
#include "Engine/ECS/Component/Particles/ParticleEmitter.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include <cmath>


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	namespace
	{
		// Guards against a divide-by-zero in age / lifetime when an effect is configured
		// with a zero lifetime. Particle-specific, so deliberately not a shared constant.
		constexpr float MIN_LIFETIME = 0.0001f;
	}


	ParticleEmitter::ParticleEmitter(ParticleEmitterProperties properties)
		: properties(std::move(properties))
	{
	}


	void ParticleEmitter::EmitParticles(int count)
	{
		if (count <= 0)
			return;

		auto remainingCapacity = properties.maxParticles - static_cast<int>(particles.size());

		auto spawnCount = count < remainingCapacity ? count : remainingCapacity;

		if (spawnCount <= 0)
			return;

		auto origin = GetEmissionOrigin();

		for (auto i = 0; i < spawnCount; ++i)
		{
			SpawnParticle(origin);
		}
	}

	void ParticleEmitter::SpawnParticle(const Vector2F& origin)
	{
		auto offset = Vector2F::Zero;

		switch (properties.shape)
		{
		case EmitterShape::CIRCLE:
			offset = random.InsideUnitCircle() * properties.shapeSize.x;
			break;

		case EmitterShape::BOX:
			offset = Vector2F(
				random.Range(-properties.shapeSize.x, properties.shapeSize.x),
				random.Range(-properties.shapeSize.y, properties.shapeSize.y));
			break;

		default:
			break;
		}

		auto direction = Vector2F::Zero;

		if (properties.radialVelocity && !offset.IsZero())
		{
			direction = offset;

			direction.Normalize();
		}
		else
		{
			auto spread = properties.radialVelocity ? 360.0f : properties.directionSpread;

			auto centre = properties.radialVelocity ? 0.0f : properties.directionAngle;

			auto angle = centre + random.Range(-spread * 0.5f, spread * 0.5f);

			auto radians = MathUtils::ToRadians(angle);

			direction = Vector2F(std::cos(radians), std::sin(radians));
		}

		auto speed = random.Range(properties.startSpeedMin, properties.startSpeedMax);

		auto lifetime = random.Range(properties.lifetimeMin, properties.lifetimeMax);

		if (lifetime < MIN_LIFETIME)
		{
			lifetime = MIN_LIFETIME;
		}

		particles.push_back(Particle
			{
				.position = origin + offset,
				.velocity = direction * speed,
				.rotation = random.Range(properties.startRotationMin, properties.startRotationMax),
				.angularVelocity = random.Range(properties.angularVelocityMin, properties.angularVelocityMax),
				.age = 0.0f,
				.lifetime = lifetime,
				.startSize = random.Range(properties.startSizeMin, properties.startSizeMax)
			});
	}

	void ParticleEmitter::IntegrateParticles(float deltaTime)
	{
		auto gravity = properties.gravity;

		auto dampingFactor = 1.0f - properties.damping * deltaTime;

		if (dampingFactor < 0.0f)
		{
			dampingFactor = 0.0f;
		}

		// Swap-removal keeps the live range dense but scrambles spawn order, so draw order
		// is arbitrary. Additive blending is order-independent, and alpha-blended particles
		// overlap with near-identical colours, so the artifact is not visible.
		for (size_t i = 0; i < particles.size(); )
		{
			auto& particle = particles[i];

			particle.age += deltaTime;

			if (particle.age >= particle.lifetime)
			{
				particle = particles.back();

				particles.pop_back();

				continue;
			}

			particle.velocity = (particle.velocity + gravity * deltaTime) * dampingFactor;

			particle.position += particle.velocity * deltaTime;

			particle.rotation += particle.angularVelocity * deltaTime;

			++i;
		}
	}

	void ParticleEmitter::BuildBatch()
	{
		auto& batch = batchData.spriteBatch;

		// clear() keeps the capacity reserved in Init, so this buffer is allocated once.
		batch.clear();

		// World-space particles already carry an absolute position; local-space ones are
		// stored as offsets and follow the emitter every frame.
		auto origin = properties.simulationSpace == SimulationSpace::LOCAL
			? transform->GetWorldPosition()
			: Vector2F::Zero;

		for (const auto& particle : particles)
		{
			auto normalizedLife = particle.age / particle.lifetime;

			auto size = MathUtils::Lerp(particle.startSize, particle.startSize * properties.endSizeMultiplier, normalizedLife);

			auto halfSize = size * 0.5f;

			auto rotation = properties.alignRotationToVelocity
				? MathUtils::ToDegrees(std::atan2(particle.velocity.y, particle.velocity.x))
				: particle.rotation;

			// Centre-anchored to match SpriteRenderer. Transform scale is deliberately
			// ignored so particle size stays in the pixels the effect was authored in.
			batch.push_back(SpriteRenderData
				{
					.texture = texture,
					.srcRect = properties.sourceRect,
					.destRect = RectF
					{
						.x = origin.x + particle.position.x - halfSize,
						.y = origin.y + particle.position.y - halfSize,
						.w = size,
						.h = size
					},
					.rotation = rotation,
					.colorMod = Color::Lerp(properties.startColor, properties.endColor, normalizedLife),
					.blendMode = properties.blendMode
				});
		}
	}

	Vector2F ParticleEmitter::GetEmissionOrigin() const
	{
		return (properties.simulationSpace == SimulationSpace::LOCAL)
			? properties.emitOffset
			: transform->GetWorldPosition() + properties.emitOffset;
	}


	void ParticleEmitter::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		textureManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().textureManager, NAME_OF(textureManager));

		if (properties.randomSeed != 0u)
		{
			random.SetSeed(properties.randomSeed);
		}

		if (properties.maxParticles < 0)
		{
			properties.maxParticles = 0;
		}

		particles.reserve(static_cast<size_t>(properties.maxParticles));
		batchData.spriteBatch.reserve(static_cast<size_t>(properties.maxParticles));

		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = properties.sortOrder;

		if (!properties.texturePath.empty())
		{
			texture = textureManager->LoadTexture(properties.texturePath);
		}
	}

	void ParticleEmitter::Start()
	{
		if (!properties.playOnStart)
			return;

		Play();
	}

	void ParticleEmitter::Update(float deltaTime)
	{
		if (!isPlaying)
			return;

		if (isEmitting)
		{
			elapsedTime += deltaTime;

			emissionAccumulator += properties.emissionRate * deltaTime;

			// The fractional remainder must carry across frames. Truncating it would make
			// any rate below one particle per frame emit nothing at all, forever.
			auto wholeParticles = static_cast<int>(emissionAccumulator);

			emissionAccumulator -= static_cast<float>(wholeParticles);

			EmitParticles(wholeParticles);

			if (properties.duration > 0.0f && elapsedTime >= properties.duration)
			{
				if (properties.loop)
				{
					elapsedTime -= properties.duration;

					EmitParticles(properties.burstCount);
				}
				else
				{
					isEmitting = false;
				}
			}
		}

		IntegrateParticles(deltaTime);

		if (!isEmitting && particles.empty())
		{
			isPlaying = false;
		}
	}

	void ParticleEmitter::Draw()
	{
		BuildBatch();

		if (texture == 0 || batchData.spriteBatch.empty())
			return;

		// Copied rather than moved: moving would hand the reserved buffer to the render
		// task and force this emitter to grow a fresh one every frame.
		renderTask.renderData = batchData;

		RenderSystem::Submit(renderTask);
	}

	void ParticleEmitter::Play()
	{
		isPlaying = true;
		isEmitting = true;
		hasPlayed = true;

		elapsedTime = 0.0f;
		emissionAccumulator = 0.0f;

		EmitParticles(properties.burstCount);

		if (properties.duration <= 0.0f && !properties.loop)
			isEmitting = false;
	}

	void ParticleEmitter::Stop(bool clearParticles)
	{
		isEmitting = false;

		if (!clearParticles)
			return;

		particles.clear();

		isPlaying = false;
	}

	void ParticleEmitter::Burst(int count)
	{
		EmitParticles(count);

		if (particles.empty())
			return;

		// A manual burst has to leave the emitter playing, otherwise Update would bail
		// out early and the particles it just released would never simulate or die.
		isPlaying = true;
		hasPlayed = true;
	}

	bool ParticleEmitter::IsPlaying() const
	{
		return isPlaying;
	}

	bool ParticleEmitter::IsFinished() const
	{
		return hasPlayed && !isPlaying;
	}

	size_t ParticleEmitter::GetAliveCount() const
	{
		return particles.size();
	}

	int ParticleEmitter::GetSortOrder() const
	{
		return renderTask.sortOrder;
	}

	TextureID ParticleEmitter::GetTexture() const
	{
		return texture;
	}

	const std::vector<Particle>& ParticleEmitter::GetParticles() const
	{
		return particles;
	}

	const SpriteBatchRenderData& ParticleEmitter::GetBatchData() const
	{
		return batchData;
	}

	const ParticleEmitterProperties& ParticleEmitter::GetProperties() const
	{
		return properties;
	}

	void ParticleEmitter::SetProperties(const ParticleEmitterProperties& value)
	{
		auto seedChanged = value.randomSeed != properties.randomSeed;
		auto textureChanged = value.texturePath != properties.texturePath;
		auto capacityChanged = value.maxParticles != properties.maxParticles;

		properties = value;

		if (properties.maxParticles < 0)
		{
			properties.maxParticles = 0;
		}

		renderTask.sortOrder = properties.sortOrder;

		// Reseed only on an actual change. A debug slider calling this every frame must not
		// restart the sequence, or every frame spawns the same particle and the effect freezes.
		if (seedChanged && properties.randomSeed != 0u)
		{
			random.SetSeed(properties.randomSeed);
		}

		if (capacityChanged)
		{
			auto capacity = static_cast<size_t>(properties.maxParticles);

			if (particles.size() > capacity)
			{
				particles.resize(capacity);
			}

			particles.reserve(capacity);
			batchData.spriteBatch.reserve(capacity);
		}

		// textureManager stays null until Init runs, so a call made before then simply
		// leaves the load to Init.
		if (textureChanged && textureManager != nullptr)
		{
			texture = properties.texturePath.empty() ? 0 : textureManager->LoadTexture(properties.texturePath);
		}
	}
}