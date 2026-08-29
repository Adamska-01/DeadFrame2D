#pragma once


namespace DF2D::Data
{
	/** @brief Frame of reference particles are simulated in. */
	enum class SimulationSpace
	{
		/** @brief Particles are detached from the emitter once spawned. */
		WORLD,

		/** @brief Particles follow the emitter's transform for their whole lifetime. */
		LOCAL
	};
}