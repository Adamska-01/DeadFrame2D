#pragma once
#include "Core/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include <coroutine>


class WaitForSeconds : public ICoroutineAwaitable
{
private:
	float timeRemaining;

	std::coroutine_handle<> continuation;


public:
	explicit WaitForSeconds(float seconds);


	bool await_ready() const noexcept override;

	void await_suspend(std::coroutine_handle<> h) override;

	void await_resume() const noexcept override;


	bool Tick(float deltaTime) override;
};