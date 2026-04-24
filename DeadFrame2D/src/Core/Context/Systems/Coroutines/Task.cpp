#include "Core/Context/Systems/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include <algorithm>


namespace DeadFrame2D::Core
{
	// Necessary when running multi-threaded coroutines
	thread_local Task* currentTask = nullptr;


	/// =========================================================================================
	/// ========================================= Task =========================================
	/// =========================================================================================
	void Task::SetCurrentTask(Task* task)
	{
		currentTask = task;
	}

	Task::Task(std::coroutine_handle<promise_type> promiseHandle)
		: promiseHandle(promiseHandle)
	{
		awaitables.clear();
	}

	Task::Task(Task&& other) noexcept
		: promiseHandle(other.promiseHandle), 
		awaitables(std::move(other.awaitables))
	{
		other.promiseHandle = nullptr;
	}

	Task::~Task()
	{
		for (auto* awaitable : awaitables)
		{
			delete awaitable;
		}

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

	void Task::AddAwaitable(ICoroutineAwaitable* awaitable)
	{
		awaitable->SetOwner(this);
		awaitables.push_back(awaitable);
	}

	bool Task::TickAwaitables(float deltaTime)
	{
		if (IsCancelled())
			return true; // Task should be removed

		std::vector<ICoroutineAwaitable*> finished;

		// Pass 1: Tick and collect finished
		for (auto* awaitable : awaitables)
		{
			if (awaitable->Tick(deltaTime))
			{
				finished.push_back(awaitable);
			}
		}

		// Pass 2: Erase and delete
		for (auto* awaitable : finished)
		{
			auto it = std::find(awaitables.begin(), awaitables.end(), awaitable);
			if (it != awaitables.end())
			{
				awaitables.erase(it);
				delete awaitable;
			}
		}

		return IsDone() || IsCancelled();
	}

	Task* Task::GetCurrentTask()
	{
		return currentTask;
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