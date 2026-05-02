#include "Core/Services/Time/FrameTimer.h"
#include <algorithm>
#include <thread>


namespace DF2D::Core
{
	using namespace std::chrono;


	float FrameTimer::deltaTime = 0;

	float FrameTimer::timeScale = 1.0f;

	int FrameTimer::currentFPS = 0;

	std::chrono::duration<float> FrameTimer::workTime;


	FrameTimer::FrameTimer()
		: countedFrames(0),
		counterDelay(0),
		isFpsLocked(true)
	{
		start = system_clock::now();
		end = start;

		SetTargetFramerate(60);
	}

	void FrameTimer::CalculateFPS()
	{
		countedFrames++;
		counterDelay += workTime.count();

		if (counterDelay >= 1000.0f)
		{
			deltaTime = (1.0f / countedFrames);
			currentFPS = countedFrames - 1;

			counterDelay = 0;
			countedFrames = 0;
		}
	}

	void FrameTimer::StartClock()
	{
		start = system_clock::now();
	}

	void FrameTimer::EndClock()
	{
		end = system_clock::now();

		workTime = duration<float>(end - start); // seconds
		deltaTime = workTime.count() * timeScale;

		// Update FPS
		countedFrames++;
		counterDelay += workTime.count();

		if (counterDelay >= 1.0f) // 1 second
		{
			currentFPS = countedFrames;
			countedFrames = 0;
			counterDelay = 0.0f;
		}
	}

	void FrameTimer::DelayByFrameTime()
	{
		if (!isFpsLocked)
			return;

		auto targetDelta = frameTime; // seconds per frame
		auto remainingTime = targetDelta - workTime.count();

		if (remainingTime > 0.0f)
		{
			auto sleepDuration = duration<float>(remainingTime);

			std::this_thread::sleep_for(duration_cast<milliseconds>(sleepDuration));

			workTime += duration<float>(remainingTime);

			deltaTime = workTime.count() * timeScale;
		}
	}

	void FrameTimer::SetTargetFramerate(unsigned int fps)
	{
		frameTime = 1.0f / static_cast<float>(fps); // seconds per frame

		isFpsLocked = true;
	}

	void FrameTimer::UnlockFramerate()
	{
		isFpsLocked = false;
	}

	float FrameTimer::DeltaTime()
	{
		return deltaTime * timeScale;
	}

	float FrameTimer::DeltaTimeUnscaled()
	{
		return workTime.count();
	}

	int FrameTimer::Framerate()
	{
		return currentFPS;
	}

	void FrameTimer::SetTimeScale(float scale)
	{
		timeScale = std::max(0.0f, scale);
	}

	float FrameTimer::GetTimeScale()
	{
		return timeScale;
	}
}