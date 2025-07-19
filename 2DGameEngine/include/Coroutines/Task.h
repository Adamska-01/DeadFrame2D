#pragma once
#include <coroutine>
#include <memory>
#include <vector>


class ICoroutineAwaitable;


struct Task : std::enable_shared_from_this<Task>
{
	struct promise_type
	{
		struct final_awaiter
		{
			std::coroutine_handle<> continuation;


			bool await_ready() const noexcept;

			void await_suspend(std::coroutine_handle<promise_type> promiseHandle) const noexcept;
			
			void await_resume() const noexcept;
		};


		std::atomic<bool> cancelled{ false };

		std::coroutine_handle<> continuation = nullptr;


		Task get_return_object();

		std::suspend_always initial_suspend() noexcept;

		final_awaiter final_suspend() noexcept;

		void return_void() noexcept;

		void unhandled_exception();

		void cancel() noexcept;

		bool is_cancelled() const noexcept;
	};


	// Necessary when running multi-threaded coroutines
	static thread_local Task* currentTask;


	std::coroutine_handle<promise_type> promiseHandle;

	std::vector<ICoroutineAwaitable*> awaitables;


	explicit Task(std::coroutine_handle<promise_type> promiseHandle);
	
	Task(Task&& other) noexcept;
	
	~Task();

	Task& operator=(Task&& other) noexcept;


	bool await_ready() const noexcept;

	void await_suspend(std::coroutine_handle<> awaiting) noexcept;

	void await_resume() const noexcept;


	bool IsDone() const;

	bool IsCancelled() const;

	void Cancel();

	void AddAwaitable(ICoroutineAwaitable* awaitable);

	bool TickAwaitables(float deltaTime);
};