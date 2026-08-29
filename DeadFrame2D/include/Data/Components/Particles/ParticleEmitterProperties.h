#pragma once
#include "Constants/CommonColors.h"
#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/Particles/EmitterShape.h"
#include "Data/Components/Particles/SimulationSpace.h"
#include "Data/Systems/Rendering/BlendMode.h"
#include <optional>
#include <string_view>


namespace DF2D::Data
{
	/** @brief Configuration data for a particle emitter. */
	struct ParticleEmitterProperties
	{
		/** @brief Texture drawn for every particle. */
		std::string_view texturePath;

		/** @brief Sub-region of the texture to draw, for atlased particle art. */
		std::optional<Core::RectI> sourceRect = std::nullopt;


		/** @brief Length of one emission cycle in seconds. Zero with loop disabled gives a pure burst. */
		float duration = 1.0f;

		/** @brief Whether emission restarts once the duration elapses. */
		bool loop = true;

		/** @brief Continuous emission rate in particles per second. */
		float emissionRate = 20.0f;

		/** @brief Particles released instantly on Play and on every loop cycle. */
		int burstCount = 0;

		/** @brief Hard cap on live particles. Also the reserved buffer size, so it never reallocates. Emission beyond the cap is dropped. */
		int maxParticles = 128;

		/** @brief Whether the emitter starts playing as soon as it is added. */
		bool playOnStart = true;


		/** @brief Region spawn positions are picked from. */
		EmitterShape shape = EmitterShape::POINT;

		/** @brief CIRCLE uses x as the radius. BOX uses both components as half-extents. */
		Core::Vector2F shapeSize = Core::Vector2F::Zero;

		/** @brief Spawn offset from the emitter's transform. */
		Core::Vector2F emitOffset = Core::Vector2F::Zero;


		/** @brief Centre of the emission cone in degrees. -90 points up in the engine's y-down space. */
		float directionAngle = -90.0f;

		/** @brief Total width of the emission cone in degrees. */
		float directionSpread = 30.0f;

		/** @brief Fires particles outward from the shape centre, ignoring directionAngle. */
		bool radialVelocity = false;

		/** @brief Lower bound of the initial speed in pixels per second. */
		float startSpeedMin = 20.0f;

		/** @brief Upper bound of the initial speed in pixels per second. */
		float startSpeedMax = 60.0f;


		/** @brief Lower bound of a particle's lifetime in seconds. */
		float lifetimeMin = 0.5f;

		/** @brief Upper bound of a particle's lifetime in seconds. */
		float lifetimeMax = 1.0f;


		/** @brief Lower bound of the spawn size in pixels. */
		float startSizeMin = 4.0f;

		/** @brief Upper bound of the spawn size in pixels. */
		float startSizeMax = 8.0f;

		/** @brief Size at death, as a multiple of the particle's own spawn size. */
		float endSizeMultiplier = 1.0f;


		/** @brief Lower bound of the spawn rotation in degrees. */
		float startRotationMin = 0.0f;

		/** @brief Upper bound of the spawn rotation in degrees. */
		float startRotationMax = 0.0f;

		/** @brief Lower bound of the rotation speed in degrees per second. */
		float angularVelocityMin = 0.0f;

		/** @brief Upper bound of the rotation speed in degrees per second. */
		float angularVelocityMax = 0.0f;

		/** @brief Points each particle along its own velocity, overriding its rotation. */
		bool alignRotationToVelocity = false;


		/** @brief Colour and alpha at spawn. */
		Core::Color startColor = Constants::CommonColors::WHITE;

		/** @brief Colour and alpha at death. Drop alpha to zero to fade out. */
		Core::Color endColor = Core::Color{ .r = 255, .g = 255, .b = 255, .a = 0 };


		/** @brief Constant acceleration in pixels per second squared. */
		Core::Vector2F gravity = Core::Vector2F::Zero;

		/** @brief Fraction of a particle's speed lost per second. */
		float damping = 0.0f;


		/** @brief How particles composite against the render target. */
		BlendMode blendMode = BlendMode::ALPHA;

		/** @brief Whether particles detach from the emitter once spawned. */
		SimulationSpace simulationSpace = SimulationSpace::WORLD;

		/** @brief Render sort order for the whole emitter. */
		int sortOrder = Constants::DefaultSortOrders::PARTICLE_RENDERER;


		/** @brief Fixed seed for reproducible output. Zero picks a fresh seed per emitter. */
		uint32_t randomSeed = 0;
	};
}