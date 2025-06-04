#pragma once
#include <coroutine>


struct Task 
{
	struct promise_type
	{
		Task get_return_object();

		std::suspend_never initial_suspend() noexcept;

		std::suspend_always final_suspend() noexcept;

		void return_void() noexcept;

		void unhandled_exception();
	};


	std::coroutine_handle<promise_type> promiseHandle;


	explicit Task(std::coroutine_handle<promise_type> promiseHandle);

	Task(Task&& other) noexcept;

	~Task();

	
	Task& operator=(Task&& other) noexcept;


	bool IsDone() const;
};