#pragma once
#include "Coroutines/CoroutineScheduler.h"
#include "Coroutines/WaitForSeconds.h"
#include "Coroutines/WaitOneFrame.h"


namespace Tools::Helpers::Coroutines
{
	inline WaitOneFrame& WaitFrame()
	{
		if (!Task::currentTask)
		{
			throw std::runtime_error("WaitFrame must be called inside a running Task.");
		}

		auto* awaitable = new WaitOneFrame();

		Task::currentTask->AddAwaitable(awaitable);

		return *awaitable;
	}

	inline WaitForSeconds& WaitSeconds(float seconds)
	{
		if (!Task::currentTask)
		{
			throw std::runtime_error("WaitSeconds must be called inside a running Task.");
		}

		auto* awaitable = new WaitForSeconds(seconds);

		Task::currentTask->AddAwaitable(awaitable);

		return *awaitable;
	}
}