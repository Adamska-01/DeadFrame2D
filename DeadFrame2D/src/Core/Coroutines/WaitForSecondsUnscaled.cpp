#include "Core/Coroutines/WaitForSecondsUnscaled.h"
#include "Core/Framerate/FrameTimer.h"


namespace DeadFrame2D::Core
{
	WaitForSecondsUnscaled::WaitForSecondsUnscaled(float seconds)
		: timeRemaining(seconds),
		continuation(nullptr)
	{
	}

	bool WaitForSecondsUnscaled::await_ready() const noexcept
	{
		return timeRemaining <= 0.0f;
	}

	void WaitForSecondsUnscaled::await_suspend(std::coroutine_handle<> h)
	{
		continuation = h;
	}

	void WaitForSecondsUnscaled::await_resume() const noexcept
	{
	}

	bool WaitForSecondsUnscaled::Tick(float deltaTime)
	{
		auto unscaledDeltaTime = FrameTimer::DeltaTimeUnscaled();

		timeRemaining -= unscaledDeltaTime;

		if (timeRemaining <= 0.0f)
		{
			if (continuation != nullptr)
			{
				continuation.resume();
			}

			return true;
		}

		return false;
	}
}