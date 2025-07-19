#pragma once
#include "Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Coroutines/Task.h"
#include <coroutine>


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