#include "Coroutines/WaitForSeconds.h"


WaitForSeconds::WaitForSeconds(float seconds)
	: timeRemaining(seconds),
	continuation(nullptr)
{
}

bool WaitForSeconds::await_ready() const noexcept
{
	return timeRemaining <= 0.0f;
}

void WaitForSeconds::await_suspend(std::coroutine_handle<> h)
{
	continuation = h;
}

void WaitForSeconds::await_resume() const noexcept
{
}

bool WaitForSeconds::Tick(float deltaTime)
{
	timeRemaining -= deltaTime;

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