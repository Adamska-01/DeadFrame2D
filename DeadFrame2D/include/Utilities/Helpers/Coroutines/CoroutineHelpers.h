#pragma once
#include "Core/Coroutines/WaitForSeconds.h"
#include "Core/Coroutines/WaitOneFrame.h"


namespace DeadFrame2D::Utilities
{
	inline DeadFrame2D::Core::WaitOneFrame& WaitFrame()
	{
		if (!DeadFrame2D::Core::Task::GetCurrentTask())
		{
			throw std::runtime_error("WaitFrame must be called inside a running Task.");
		}

		auto* awaitable = new DeadFrame2D::Core::WaitOneFrame();

		DeadFrame2D::Core::Task::GetCurrentTask()->AddAwaitable(awaitable);

		return *awaitable;
	}

	inline DeadFrame2D::Core::WaitForSeconds& WaitSeconds(float seconds)
	{
		if (!DeadFrame2D::Core::Task::GetCurrentTask())
		{
			throw std::runtime_error("WaitSeconds must be called inside a running Task.");
		}

		auto* awaitable = new DeadFrame2D::Core::WaitForSeconds(seconds);

		DeadFrame2D::Core::Task::GetCurrentTask()->AddAwaitable(awaitable);

		return *awaitable;
	}
}