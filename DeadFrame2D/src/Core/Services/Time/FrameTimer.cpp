#include "Core/Services/Time/FrameTimer.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>


namespace DF2D::Core
{
	using namespace std::chrono;
	using namespace DF2D::Utilities;


	constexpr float DEFAULT_TARGET_FRAMERATE = 60.0f;

	constexpr float FPS_SAMPLE_WINDOW_SECONDS = 1.0f;


	FrameTimer::FrameTimer(std::optional<int> targetFramerate, std::unique_ptr<IClock> clock)
		: clock(std::move(clock)),
		start(nanoseconds::zero()),
		frameDuration(0.0f),
		timeScale(1.0f),
		targetFrameTime(1.0f / DEFAULT_TARGET_FRAMERATE),
		counterDelay(0.0f),
		countedFrames(0),
		currentFPS(0),
		isFpsLocked(false)
	{
		Guard::AgainstNull(this->clock.get(), NAME_OF(clock));

		start = this->clock->Now();

		if (targetFramerate.has_value() && *targetFramerate > 0)
		{
			SetTargetFramerate(static_cast<unsigned int>(*targetFramerate));
		}
		else
		{
			UnlockFramerate();
		}
	}


	void FrameTimer::AccumulateFrame(float seconds)
	{
		counterDelay += seconds;

		if (counterDelay < FPS_SAMPLE_WINDOW_SECONDS)
			return;

		currentFPS = countedFrames;

		countedFrames = 0;
		counterDelay = 0.0f;
	}


	void FrameTimer::StartClock()
	{
		start = clock->Now();
	}

	void FrameTimer::EndClock()
	{
		frameDuration = duration_cast<duration<float>>(clock->Now() - start);

		countedFrames++;

		AccumulateFrame(frameDuration.count());
	}

	void FrameTimer::DelayByFrameTime()
	{
		if (!isFpsLocked)
			return;

		auto remainingTime = targetFrameTime - frameDuration.count();

		if (remainingTime <= 0.0f)
			return;

		auto sleepDuration = duration<float>(remainingTime);

		clock->SleepFor(duration_cast<nanoseconds>(sleepDuration));

		// The slept time is part of the frame, so both the frame delta and the FPS window must see it
		frameDuration += sleepDuration;

		AccumulateFrame(remainingTime);
	}


	void FrameTimer::SetTargetFramerate(unsigned int fps)
	{
		if (fps == 0)
			return;

		targetFrameTime = 1.0f / static_cast<float>(fps); // seconds per frame

		isFpsLocked = true;
	}

	void FrameTimer::UnlockFramerate()
	{
		isFpsLocked = false;
	}

	bool FrameTimer::IsFramerateLocked() const
	{
		return isFpsLocked;
	}

	int FrameTimer::Framerate() const
	{
		return currentFPS;
	}


	void FrameTimer::SetTimeScale(float scale)
	{
		timeScale = std::max(0.0f, scale);
	}

	float FrameTimer::GetTimeScale() const
	{
		return timeScale;
	}


	float FrameTimer::DeltaTime() const
	{
		return frameDuration.count() * timeScale;
	}

	float FrameTimer::DeltaTimeUnscaled() const
	{
		return frameDuration.count();
	}
}