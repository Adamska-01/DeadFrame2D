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


Task Task::promise_type::get_return_object()
{
	return Task(std::coroutine_handle<promise_type>::from_promise(*this));
}

std::suspend_never Task::promise_type::initial_suspend() noexcept
{
	return {};
}

std::suspend_always Task::promise_type::final_suspend() noexcept
{
	return {};
}

void Task::promise_type::return_void() noexcept
{
}

void Task::promise_type::unhandled_exception()
{
	std::terminate();
}
