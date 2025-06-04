#pragma once
#include "Coroutines/Abstractions/ICoroutineAwaitable.h"
#include <coroutine>


class WaitOneFrame : public ICoroutineAwaitable 
{
private:
	std::coroutine_handle<> continuation;


public:
	WaitOneFrame() = default;


	bool await_ready() const noexcept;

	void await_suspend(std::coroutine_handle<> h);

	void await_resume() const noexcept;


	bool Tick(float) override;
};