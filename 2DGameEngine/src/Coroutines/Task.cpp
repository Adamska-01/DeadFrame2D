#include "Coroutines/Task.h"
#include <exception>


Task::Task(std::coroutine_handle<promise_type> promiseHandle)
	: promiseHandle(promiseHandle)
{
}

Task::Task(Task&& other) noexcept
	: promiseHandle(other.promiseHandle) 
{
	other.promiseHandle = nullptr;
}

Task::~Task()
{
	if (promiseHandle == nullptr)
		return;

	promiseHandle.destroy();
	promiseHandle = nullptr;
}

Task& Task::operator=(Task&& other) noexcept
{
	if (this == &other)
		return *this;

	if (promiseHandle)
	{
		promiseHandle.destroy();
	}
	
	promiseHandle = other.promiseHandle;
	other.promiseHandle = nullptr;
}

bool Task::IsDone() const
{
	return promiseHandle == nullptr || promiseHandle.done();
}

bool Task::await_ready() const noexcept
{
	return false;
}

void Task::await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
{
	promiseHandle.promise().continuation = awaitingCoroutine;
}

void Task::await_resume() const noexcept
{

}


Task Task::promise_type::get_return_object()
{
	return Task(std::coroutine_handle<promise_type>::from_promise(*this));
}

std::suspend_never Task::promise_type::initial_suspend() noexcept
{
	return {};
}

Task::promise_type::Awaiter Task::promise_type::final_suspend() noexcept
{
	return Awaiter{ continuation };
}

void Task::promise_type::return_void() noexcept
{
}

void Task::promise_type::unhandled_exception()
{
	std::terminate();
}


bool Task::promise_type::Awaiter::await_ready() const noexcept
{
	return false;
}

void Task::promise_type::Awaiter::await_suspend(std::coroutine_handle<>) const noexcept
{
	if (continuation == nullptr)
		return;

	continuation.resume();
}

void Task::promise_type::Awaiter::await_resume() const noexcept
{

}