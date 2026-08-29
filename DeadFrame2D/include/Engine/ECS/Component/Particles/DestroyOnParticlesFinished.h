#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


namespace DF2D::Engine
{
	class ParticleEmitter;


	/**
	 * @brief Destroys the owning GameObject once its ParticleEmitter has stopped emitting
	 * and outlived its last particle. Intended for fire-and-forget one-shot effects.
	 *
	 * The alternative is GameObject::Destroy(float) handed to the CoroutineScheduler, but
	 * that couples the spawn site to the effect's tuning: retune lifetimeMax and the object
	 * disappears mid-fade. This component needs no such arithmetic.
	 */
	class DF2D_API DestroyOnParticlesFinished : public GameComponent
	{
		TYPE_INFO(DestroyOnParticlesFinished, GameComponent);


	private:
		ComponentHandle<ParticleEmitter> emitter;


	public:
		DestroyOnParticlesFinished();

		virtual ~DestroyOnParticlesFinished() override = default;


		virtual void Init() override;

		virtual void Update(float deltaTime) override;
	};
}