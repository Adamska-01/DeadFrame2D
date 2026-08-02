#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include "Core/Services/Time/Abstractions/ITimeProvider.h"
#include "DF2D_API.h"
#include <chrono>
#include <memory>
#include <optional>


namespace DF2D::Core
{
	class DF2D_API FrameTimer : public ITimeProvider
	{
	private:
		std::unique_ptr<IClock> clock;

		std::chrono::nanoseconds start;

		std::chrono::duration<float> frameDuration;

		float timeScale;

		float targetFrameTime;

		float counterDelay;

		int countedFrames;

		int currentFPS;

		bool isFpsLocked;


		void AccumulateFrame(float seconds);


	public:
		FrameTimer(std::optional<int> targetFramerate, std::unique_ptr<IClock> clock);

		~FrameTimer() override = default;

		FrameTimer(const FrameTimer&) = delete;

		FrameTimer(FrameTimer&&) = delete;

		FrameTimer& operator=(const FrameTimer&) = delete;

		FrameTimer& operator=(FrameTimer&&) = delete;


		void StartClock();

		void EndClock();

		void DelayByFrameTime();


		void SetTargetFramerate(unsigned int fps);

		void UnlockFramerate();

		bool IsFramerateLocked() const;

		int Framerate() const;


		void SetTimeScale(float scale);

		float GetTimeScale() const;


		float DeltaTime() const override;

		float DeltaTimeUnscaled() const override;
	};
}