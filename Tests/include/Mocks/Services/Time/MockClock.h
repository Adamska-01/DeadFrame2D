#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include <vector>


struct MockClock : DF2D::Core::IClock
{
	// Controllable state
	mutable std::chrono::nanoseconds now{0};

	bool sleepAdvancesTime{true};

	std::chrono::nanoseconds sleepOvershoot{0};

	// Simulates a busy-spin loop making forward progress on each poll, so code under test that
	// spins on Now() terminates instead of hanging against a clock that only moves via Advance/SleepFor.
	std::chrono::nanoseconds autoAdvancePerNowCall{std::chrono::microseconds(1)};


	// Call tracking
	int sleepCount{0};

	std::vector<std::chrono::nanoseconds> sleeps;


	void Advance(float seconds)
	{
		now += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(seconds));
	}


	std::chrono::nanoseconds Now() const override
	{
		auto result = now;

		now += autoAdvancePerNowCall;

		return result;
	}

	void SleepFor(std::chrono::nanoseconds duration) override
	{
		sleepCount++;
		sleeps.push_back(duration);

		if (sleepAdvancesTime)
		{
			now += duration + sleepOvershoot;
		}
	}
};