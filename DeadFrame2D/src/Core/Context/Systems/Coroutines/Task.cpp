#include "Core/Context/Systems/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include <algorithm>


namespace DF2D::Core
{
	/// =========================================================================================
	/// ========================================= Task =========================================
	/// =========================================================================================
	Task::Task(std::coroutine_handle<promise_type> promiseHandle)
		: promiseHandle(promiseHandle)
	{
		promiseHandle.promise().owningTask = this;
	}

	Task::Task(Task&& other) noexcept
		: promiseHandle(other.promiseHandle),
		awaitables(std::move(other.awaitables))
	{
		other.promiseHandle = nullptr;

		if (promiseHandle)
		{
			promiseHandle.promise().owningTask = this;
		}
	}

	Task::~Task()
	{
		awaitables.clear();

		if (promiseHandle)
		{
			promiseHandle.destroy();
			promiseHandle = nullptr;
		}
	}

	Task& Task::operator=(Task&& other) noexcept
	{
		if (this != &other)
		{
			if (promiseHandle)
			{
				promiseHandle.destroy();
				promiseHandle = nullptr;
			}

			promiseHandle = other.promiseHandle;

			awaitables = std::move(other.awaitables);

			other.promiseHandle = nullptr;

			if (promiseHandle)
			{
				promiseHandle.promise().owningTask = this;
			}
		}

		return *this;
	}

	bool Task::await_ready() const noexcept
	{
		return false;
	}

	void Task::await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
	{
		if (promiseHandle == nullptr)
			return;

		promiseHandle.promise().continuation = awaitingCoroutine;
	}

	void Task::await_resume() const noexcept
	{

	}

	bool Task::IsDone() const
	{
		return !promiseHandle || promiseHandle.done();
	}

	bool Task::IsCancelled() const
	{
		return promiseHandle != nullptr && promiseHandle.promise().is_cancelled();
	}

	void Task::Cancel()
	{
		if (promiseHandle == nullptr)
			return;

		promiseHandle.promise().cancel();
	}

	void Task::AddAwaitable(std::unique_ptr<ICoroutineAwaitable> awaitable)
	{
		awaitables.push_back(std::move(awaitable));
	}

	bool Task::TickAwaitables(float scaledDt, float unscaledDt)
	{
		if (IsCancelled())
			return true; // Task should be removed

		std::vector<size_t> finishedIndices;
		size_t count = awaitables.size();

		for (size_t i = 0; i < count; ++i)
		{
			if (!awaitables[i]->Tick(scaledDt, unscaledDt))
				continue;

			finishedIndices.push_back(i);
		}

		for (auto it = finishedIndices.rbegin(); it != finishedIndices.rend(); ++it)
		{
			awaitables.erase(awaitables.begin() + *it);
		}

		return IsDone() || IsCancelled();
	}



	/// =========================================================================================
	/// ===================================== promise_type ======================================
	/// =========================================================================================
	Task Task::promise_type::get_return_object()
	{
		return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
	}

	std::suspend_always Task::promise_type::initial_suspend() noexcept
	{
		return {};
	}

	Task::promise_type::final_awaiter Task::promise_type::final_suspend() noexcept
	{
		return final_awaiter{ continuation };
	}

	void Task::promise_type::return_void() noexcept
	{

	}

	void Task::promise_type::unhandled_exception()
	{
		std::terminate();
	}

	void Task::promise_type::cancel() noexcept
	{
		cancelled.store(true, std::memory_order_relaxed);
	}

	bool Task::promise_type::is_cancelled() const noexcept
	{
		return cancelled.load(std::memory_order_relaxed);
	}



	/// =========================================================================================
	/// ===================================== final_awaiter =====================================
	/// =========================================================================================
	bool Task::promise_type::final_awaiter::await_ready() const noexcept
	{
		return false;
	}

	void Task::promise_type::final_awaiter::await_suspend(std::coroutine_handle<promise_type> promiseHandle) const noexcept
	{
		auto& promise = promiseHandle.promise();

		if (!promise.continuation)
			return;

		promise.continuation.resume();
	}

	void Task::promise_type::final_awaiter::await_resume() const noexcept
	{

	}
}