#pragma once
#include "Core/Math/Vector2.h"
#include <chrono>
#include <cstdint>


namespace DF2D::Core
{
	/** @brief Small deterministic xorshift32 pseudo-random number generator. */
	class Random
	{
	private:
		uint32_t state;


		static uint32_t GenerateSeed();


	public:
		Random();

		explicit Random(uint32_t seed);


		/** @brief Reseeds the generator. A seed of zero is remapped to one. */
		void SetSeed(uint32_t seed);

		/** @brief Returns the next raw 32-bit value in the sequence. */
		uint32_t NextUInt();

		/** @brief Returns a value in the half-open range [0, 1). */
		float Value();

		/** @brief Returns a value in the inclusive range [min, max]. */
		float Range(float min, float max);

		/** @brief Returns a value in the inclusive range [min, max]. */
		int Range(int min, int max);

		/** @brief Returns a uniformly distributed point inside the unit circle. */
		Vector2F InsideUnitCircle();
	};
}


namespace DF2D::Core
{
	inline uint32_t Random::GenerateSeed()
	{
		static uint32_t counter = 0u;

		const auto ticks = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());

		return ticks ^ (++counter * 2654435761u);
	}

	inline Random::Random()
	{
		SetSeed(GenerateSeed());
	}

	inline Random::Random(uint32_t seed)
	{
		SetSeed(seed);
	}

	inline void Random::SetSeed(uint32_t seed)
	{
		state = seed != 0u ? seed : 1u;
	}

	inline uint32_t Random::NextUInt()
	{
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;

		return state;
	}

	inline float Random::Value()
	{
		return static_cast<float>(NextUInt() >> 8) / 16777216.0f;
	}

	inline float Random::Range(float min, float max)
	{
		if (min > max)
			return Range(max, min);

		return min + (max - min) * Value();
	}

	inline int Random::Range(int min, int max)
	{
		if (min > max)
			return Range(max, min);

		const auto span = static_cast<uint32_t>(max - min) + 1u;

		return min + static_cast<int>(NextUInt() % span);
	}

	inline Vector2F Random::InsideUnitCircle()
	{
		while (true)
		{
			const auto x = Range(-1.0f, 1.0f);
			const auto y = Range(-1.0f, 1.0f);

			if (x * x + y * y <= 1.0f)
				return Vector2F(x, y);
		}
	}
}