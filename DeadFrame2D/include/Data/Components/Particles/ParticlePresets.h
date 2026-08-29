#pragma once
#include "Data/Components/Particles/ParticleEmitterProperties.h"
#include <string_view>


namespace DF2D::Data::ParticlePresets
{
	// Starting points tuned against a small soft round dot texture. Copy and retune per
	// game rather than treating these as final art.

	/** @brief Continuous upward plume, buoyant, additive so the dense core reads white-hot. */
	inline ParticleEmitterProperties Fire(std::string_view texturePath)
	{
		return ParticleEmitterProperties
		{
			.texturePath = texturePath,
			.duration = 1.0f,
			.loop = true,
			.emissionRate = 45.0f,
			.maxParticles = 96,
			.shape = EmitterShape::CIRCLE,
			.shapeSize = Core::Vector2F(4.0f, 0.0f),
			.directionAngle = -90.0f,
			.directionSpread = 25.0f,
			.startSpeedMin = 30.0f,
			.startSpeedMax = 55.0f,
			.lifetimeMin = 0.35f,
			.lifetimeMax = 0.7f,
			.startSizeMin = 5.0f,
			.startSizeMax = 9.0f,
			.endSizeMultiplier = 0.25f,
			.startColor = Core::Color{ .r = 255, .g = 232, .b = 140, .a = 255 },
			.endColor = Core::Color{ .r = 220, .g = 48, .b = 12, .a = 0 },
			.gravity = Core::Vector2F(0.0f, -40.0f),
			.damping = 1.2f,
			.blendMode = BlendMode::ADDITIVE
		};
	}

	/** @brief Slow tumbling plume that expands and thins as it rises. */
	inline ParticleEmitterProperties Smoke(std::string_view texturePath)
	{
		return ParticleEmitterProperties
		{
			.texturePath = texturePath,
			.duration = 1.0f,
			.loop = true,
			.emissionRate = 8.0f,
			.maxParticles = 64,
			.shape = EmitterShape::CIRCLE,
			.shapeSize = Core::Vector2F(3.0f, 0.0f),
			.directionAngle = -90.0f,
			.directionSpread = 30.0f,
			.startSpeedMin = 12.0f,
			.startSpeedMax = 28.0f,
			.lifetimeMin = 1.5f,
			.lifetimeMax = 3.0f,
			.startSizeMin = 6.0f,
			.startSizeMax = 10.0f,
			.endSizeMultiplier = 3.5f,
			.angularVelocityMin = -25.0f,
			.angularVelocityMax = 25.0f,
			.startColor = Core::Color{ .r = 90, .g = 90, .b = 95, .a = 180 },
			.endColor = Core::Color{ .r = 150, .g = 150, .b = 155, .a = 0 },
			.gravity = Core::Vector2F(0.0f, -15.0f),
			.damping = 0.8f,
			.blendMode = BlendMode::ALPHA
		};
	}

	/** @brief One-shot shower of fast streaks that arc down under heavy gravity. */
	inline ParticleEmitterProperties Sparks(std::string_view texturePath)
	{
		return ParticleEmitterProperties
		{
			.texturePath = texturePath,
			.duration = 0.0f,
			.loop = false,
			.emissionRate = 0.0f,
			.burstCount = 20,
			.maxParticles = 20,
			.radialVelocity = true,
			.startSpeedMin = 180.0f,
			.startSpeedMax = 420.0f,
			.lifetimeMin = 0.15f,
			.lifetimeMax = 0.4f,
			.startSizeMin = 2.0f,
			.startSizeMax = 3.0f,
			.endSizeMultiplier = 0.6f,
			.alignRotationToVelocity = true,
			.startColor = Core::Color{ .r = 255, .g = 240, .b = 180, .a = 255 },
			.endColor = Core::Color{ .r = 255, .g = 140, .b = 40, .a = 0 },
			.gravity = Core::Vector2F(0.0f, 700.0f),
			.damping = 1.5f,
			.blendMode = BlendMode::ADDITIVE
		};
	}

	/** @brief One-shot radial burst of shrinking debris. Pair with a flipbook sprite for the flash. */
	inline ParticleEmitterProperties Explosion(std::string_view texturePath)
	{
		return ParticleEmitterProperties
		{
			.texturePath = texturePath,
			.duration = 0.0f,
			.loop = false,
			.emissionRate = 0.0f,
			.burstCount = 48,
			.maxParticles = 48,
			.shape = EmitterShape::CIRCLE,
			.shapeSize = Core::Vector2F(6.0f, 0.0f),
			.radialVelocity = true,
			.startSpeedMin = 120.0f,
			.startSpeedMax = 260.0f,
			.lifetimeMin = 0.25f,
			.lifetimeMax = 0.55f,
			.startSizeMin = 4.0f,
			.startSizeMax = 10.0f,
			.endSizeMultiplier = 0.1f,
			.startColor = Core::Color{ .r = 255, .g = 250, .b = 210, .a = 255 },
			.endColor = Core::Color{ .r = 255, .g = 90, .b = 20, .a = 0 },
			.gravity = Core::Vector2F(0.0f, 180.0f),
			.damping = 2.5f,
			.blendMode = BlendMode::ADDITIVE
		};
	}

	/** @brief Low wide puff kicked up along the ground, drawn behind character sprites. */
	inline ParticleEmitterProperties Dust(std::string_view texturePath)
	{
		return ParticleEmitterProperties
		{
			.texturePath = texturePath,
			.duration = 1.0f,
			.loop = true,
			.emissionRate = 14.0f,
			.maxParticles = 48,
			.shape = EmitterShape::BOX,
			.shapeSize = Core::Vector2F(10.0f, 2.0f),
			.directionAngle = -90.0f,
			.directionSpread = 120.0f,
			.startSpeedMin = 10.0f,
			.startSpeedMax = 35.0f,
			.lifetimeMin = 0.3f,
			.lifetimeMax = 0.7f,
			.startSizeMin = 3.0f,
			.startSizeMax = 6.0f,
			.endSizeMultiplier = 1.8f,
			.startColor = Core::Color{ .r = 150, .g = 130, .b = 100, .a = 160 },
			.endColor = Core::Color{ .r = 150, .g = 130, .b = 100, .a = 0 },
			.gravity = Core::Vector2F(0.0f, 30.0f),
			.damping = 0.5f,
			.blendMode = BlendMode::ALPHA,
			.sortOrder = Constants::DefaultSortOrders::SPRITE_RENDERER - 1
		};
	}
}