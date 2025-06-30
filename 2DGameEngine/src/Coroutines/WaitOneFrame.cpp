#include "Coroutines/WaitOneFrame.h"


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
	continuation.resume();

	return true;
}