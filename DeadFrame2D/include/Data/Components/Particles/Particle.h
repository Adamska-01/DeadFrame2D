#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/** @brief Simulation state of a single live particle. */
	struct Particle
	{
		Core::Vector2F position = Core::Vector2F::Zero;

		Core::Vector2F velocity = Core::Vector2F::Zero;

		/** @brief Current rotation in degrees. */
		float rotation = 0.0f;

		/** @brief Rotation change in degrees per second. */
		float angularVelocity = 0.0f;

		/** @brief Seconds this particle has been alive. */
		float age = 0.0f;

		/** @brief Total seconds this particle will live for. */
		float lifetime = 1.0f;

		/** @brief Size in pixels at spawn time. */
		float startSize = 1.0f;
	};
}