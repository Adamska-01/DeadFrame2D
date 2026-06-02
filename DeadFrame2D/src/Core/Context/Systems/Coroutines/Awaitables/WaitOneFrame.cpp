#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"


namespace DF2D::Core
{
	bool WaitOneFrame::await_ready() const noexcept
	{
		return false;
	}

	void WaitOneFrame::await_suspend(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	void WaitOneFrame::await_resume() const noexcept
	{
	}

	void WaitOneFrame::SetHandle(std::coroutine_handle<> handle)
	{
		continuation = handle;
	}

	bool WaitOneFrame::Tick(float, float)
	{
		if (!continuation)
			return true;

		continuation.resume();
		continuation = nullptr;

		return true;
	}
}