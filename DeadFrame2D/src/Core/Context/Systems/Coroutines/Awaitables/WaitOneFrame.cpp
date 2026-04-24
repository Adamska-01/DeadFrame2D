#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"


namespace DeadFrame2D::Core
{
	bool WaitOneFrame::await_ready() const noexcept
	{
		return false;
	}

	void WaitOneFrame::await_suspend(std::coroutine_handle<> h)
	{
		continuation = h;
	}

	void WaitOneFrame::await_resume() const noexcept
	{
	}

	bool WaitOneFrame::Tick(float)
	{
		if (!continuation)
			return true;

		continuation.resume();
		continuation = nullptr;

		return true;
	}
}