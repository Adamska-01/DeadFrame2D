#include "Core/Services/Time/FrameTimer.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>
#include <cmath>


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
		isFpsLocked(false),
		sleepOvershootEstimate(0.0f),
		sleepOvershootMean(0.0f),
		sleepOvershootM2(0.0f),
		sleepSampleCount(0)
	{
		Guard::AgainstNull(this->clock.get(), NAME_OF(clock));

		start = this->clock->Now();

		if (targetFramerate.has_value() && *targetFramerate > 0)
		{
			SetTargetFramerate(static_cast<unsigned int>(180));
		}
		else
		{
			UnlockFramerate();
		}
	}


	void FrameTimer::BeginFrame()
	{
		start = clock->Now();
	}

	void FrameTimer::EndFrame()
	{
		frameDuration = duration_cast<duration<float>>(clock->Now() - start);

		countedFrames++;

		DelayByFrameTime();

		// frameDuration now reflects the complete frame (work + any limiter sleep)
		AccumulateFrame(frameDuration.count());
	}

	void FrameTimer::DelayByFrameTime()
	{
		if (!isFpsLocked)
			return;

		auto remainingTime = targetFrameTime - frameDuration.count();

		if (remainingTime <= 0.0f)
			return;

		auto waitStart = clock->Now();

		// Sleep for the portion we're confident SleepFor won't overshoot past, based on what it
		// has actually overshot by on this machine so far; spin the calibrated remainder for precision.
		auto requestedSleep = std::max(0.0f, remainingTime - sleepOvershootEstimate);

		if (requestedSleep > 0.0f)
		{
			auto sleepStart = clock->Now();

			clock->SleepFor(duration_cast<nanoseconds>(duration<float>(requestedSleep)));

			auto actualSleep = duration_cast<duration<float>>(clock->Now() - sleepStart).count();

			UpdateSleepOvershootEstimate(actualSleep - requestedSleep);
		}

		while (duration_cast<duration<float>>(clock->Now() - waitStart).count() < remainingTime);

		frameDuration += duration_cast<duration<float>>(clock->Now() - waitStart);
	}

	void FrameTimer::UpdateSleepOvershootEstimate(float observedOvershoot)
	{
		sleepSampleCount++;

		auto delta = observedOvershoot - sleepOvershootMean;

		sleepOvershootMean += delta / static_cast<float>(sleepSampleCount);

		sleepOvershootM2 += delta * (observedOvershoot - sleepOvershootMean);

		auto variance = sleepSampleCount > 1 ? sleepOvershootM2 / static_cast<float>(sleepSampleCount - 1) : 0.0f;

		sleepOvershootEstimate = std::max(0.0f, sleepOvershootMean + std::sqrt(variance));
	}

	void FrameTimer::AccumulateFrame(float seconds)
	{
		counterDelay += seconds;

		if (counterDelay > 0.0f)
			currentFPS = static_cast<float>(countedFrames) / counterDelay;

		if (counterDelay < FPS_SAMPLE_WINDOW_SECONDS)
			return;

		countedFrames = 0;
		counterDelay = 0.0f;
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

	float FrameTimer::Framerate() const
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