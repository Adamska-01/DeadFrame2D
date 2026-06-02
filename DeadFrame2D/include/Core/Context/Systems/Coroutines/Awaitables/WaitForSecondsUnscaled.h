#pragma once
#include "Core/Context/Systems/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "DF2D_API.h"
#include <coroutine>


namespace DF2D::Core
{
	class DF2D_API WaitForSecondsUnscaled : public ICoroutineAwaitable
	{
	private:
		float timeRemaining;

		std::coroutine_handle<> continuation;


	public:
		WaitForSecondsUnscaled(float seconds);


		bool await_ready() const noexcept override;

		void await_suspend(std::coroutine_handle<> handle) override;

		void await_resume() const noexcept override;

		void SetHandle(std::coroutine_handle<> handle) override;


		bool Tick(float, float unscaledDt) override;
	};
}