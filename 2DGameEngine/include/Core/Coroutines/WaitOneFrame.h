#pragma once
#include "Core/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Core/Coroutines/Task.h"
#include <coroutine>


namespace DeadFrame2D::Core
{
	class WaitOneFrame : public ICoroutineAwaitable
	{
	private:
		std::coroutine_handle<> continuation;


	public:
		WaitOneFrame() = default;


		bool await_ready() const noexcept override;

		void await_suspend(std::coroutine_handle<> h) override;

		void await_resume() const noexcept override;


		bool Tick(float) override;	
	};
}