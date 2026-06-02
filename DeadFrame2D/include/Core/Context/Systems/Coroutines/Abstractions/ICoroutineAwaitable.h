#pragma once
#include "Core/Context/Systems/Coroutines/Task.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	class DF2D_API ICoroutineAwaitable
	{
	public:
		virtual ~ICoroutineAwaitable() = default;


		virtual bool await_ready() const noexcept = 0;

		virtual void await_suspend(std::coroutine_handle<> h) = 0;

		virtual void await_resume() const noexcept = 0;

		virtual void SetHandle(std::coroutine_handle<> h) = 0;


		virtual bool Tick(float scaledDt, float unscaledDt) = 0;
	};
}