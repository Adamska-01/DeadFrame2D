#pragma once
#include "Coroutines/Abstractions/ICoroutineAwaitable.h"
#include <coroutine>


class WaitForSeconds : public ICoroutineAwaitable
{
private:
	float timeRemaining;

	std::coroutine_handle<> continuation;


public:
	explicit WaitForSeconds(float seconds);


	bool await_ready() const noexcept;

	void await_suspend(std::coroutine_handle<> h);

	void await_resume() const noexcept;


	bool Tick(float deltaTime) override;
};