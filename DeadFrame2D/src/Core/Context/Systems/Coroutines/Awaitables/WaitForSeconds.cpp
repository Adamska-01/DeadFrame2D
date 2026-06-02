#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSeconds.h"


namespace DF2D::Core
{
	WaitForSeconds::WaitForSeconds(float seconds)
		: timeRemaining(seconds),
		continuation(nullptr)
	{
	}


	bool WaitForSeconds::await_ready() const noexcept
	{
		return timeRemaining <= 0.0f;
	}

	void WaitForSeconds::await_suspend(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	void WaitForSeconds::await_resume() const noexcept
	{
	}

	void WaitForSeconds::SetHandle(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	bool WaitForSeconds::Tick(float scaledDt, float)
	{
		timeRemaining -= scaledDt;

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