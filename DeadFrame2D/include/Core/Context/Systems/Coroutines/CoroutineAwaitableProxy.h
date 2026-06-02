#pragma once
#include "Core/Context/Systems/Coroutines/Task.h"
#include <tuple>


namespace DF2D::Core
{
	template<typename T, typename... Args>
	struct CoroutineAwaitableProxy
	{
	private:
		std::tuple<Args...> args;


	public:
		CoroutineAwaitableProxy(Args... args)
			: args(std::move(args)...)
		{
		}


		bool await_ready() const noexcept
		{
			return false;
		}

		bool await_suspend(std::coroutine_handle<> h)
		{
			auto promiseHandle = std::coroutine_handle<Task::promise_type>::from_address(h.address());
			auto task = promiseHandle.promise().owningTask;

			auto realAwaitable = std::apply([](auto&&... args)
				{
					return std::make_unique<T>(std::forward<decltype(args)>(args)...);
				},
				args);

			if (realAwaitable->await_ready())
				return false;

			realAwaitable->SetHandle(h);

			task->AddAwaitable(std::move(realAwaitable));

			return true;
		}

		void await_resume() const noexcept
		{
		}
	};
}