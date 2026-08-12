#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include "Core/Services/Time/Abstractions/IFrameCycle.h"
#include "Core/Services/Time/Abstractions/ITimeProvider.h"
#include "DF2D_API.h"
#include <chrono>
#include <memory>
#include <optional>


namespace DF2D::Core
{
	class DF2D_API FrameTimer : public ITimeProvider, public IFrameCycle
	{
	private:
		std::unique_ptr<IClock> clock;

		std::chrono::nanoseconds start;

		std::chrono::duration<float> frameDuration;

		float timeScale;

		float targetFrameTime;

		float counterDelay;

		int countedFrames;

		float currentFPS;

		bool isFpsLocked;

		// Self-calibrating estimate of how much SleepFor overshoots its requested duration on
		// this machine, so the limiter can sleep for the safe portion of the budget and spin
		// only the calibrated tail instead of guessing a fixed margin.
		float sleepOvershootEstimate;

		float sleepOvershootMean;

		float sleepOvershootM2;

		int sleepSampleCount;


		void BeginFrame() override;

		void EndFrame() override;


		void DelayByFrameTime();

		void AccumulateFrame(float seconds);

		void UpdateSleepOvershootEstimate(float observedOvershoot);


	public:
		FrameTimer(std::optional<int> targetFramerate, std::unique_ptr<IClock> clock);

		~FrameTimer() override = default;

		FrameTimer(const FrameTimer&) = delete;

		FrameTimer(FrameTimer&&) = delete;

		FrameTimer& operator=(const FrameTimer&) = delete;

		FrameTimer& operator=(FrameTimer&&) = delete;


		void SetTargetFramerate(unsigned int fps) override;

		void UnlockFramerate() override;

		bool IsFramerateLocked() const override;

		float Framerate() const override;


		void SetTimeScale(float scale) override;

		float GetTimeScale() const override;


		float DeltaTime() const override;

		float DeltaTimeUnscaled() const override;
	};
}