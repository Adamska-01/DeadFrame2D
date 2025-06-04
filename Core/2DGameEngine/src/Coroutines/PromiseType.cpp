#include "Coroutines/PromiseType.h"
#include "Coroutines/Task.h"
#include <exception>


Task PromiseType::get_return_object()
{
	return Task(std::coroutine_handle<PromiseType>::from_promise(*this));
}

std::suspend_never PromiseType::initial_suspend() noexcept
{
	return {};
}

std::suspend_never PromiseType::final_suspend() noexcept
{
	return {};
}

void PromiseType::return_void() noexcept
{
}

void PromiseType::unhandled_exception()
{
	std::terminate();
}