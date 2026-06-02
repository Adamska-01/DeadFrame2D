#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSecondsUnscaled.h"


namespace DF2D::Core
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

	void WaitForSecondsUnscaled::await_suspend(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	void WaitForSecondsUnscaled::await_resume() const noexcept
	{
	}

	void WaitForSecondsUnscaled::SetHandle(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	bool WaitForSecondsUnscaled::Tick(float, float unscaledDt)
	{
		timeRemaining -= unscaledDt;

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