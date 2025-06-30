#pragma once
#include <coroutine>


struct Task 
{
	struct promise_type
	{
		struct Awaiter
		{
			std::coroutine_handle<> continuation;


			bool await_ready() const noexcept;

			void await_suspend(std::coroutine_handle<>) const noexcept;

			void await_resume() const noexcept;
		};


		std::coroutine_handle<> continuation = nullptr;
		

		Task get_return_object();

		std::suspend_never initial_suspend() noexcept;

		Awaiter final_suspend() noexcept;

		void return_void() noexcept;

		void unhandled_exception();
	};


	std::coroutine_handle<promise_type> promiseHandle;


	explicit Task(std::coroutine_handle<promise_type> promiseHandle);

	Task(Task&& other) noexcept;

	~Task();

	
	Task& operator=(Task&& other) noexcept;


	bool IsDone() const;


	bool await_ready() const noexcept;

	void await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept;

	void await_resume() const noexcept;
};