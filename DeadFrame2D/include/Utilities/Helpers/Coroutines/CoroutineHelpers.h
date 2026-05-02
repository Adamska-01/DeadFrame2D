#pragma once
#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSeconds.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSecondsUnscaled.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"


namespace DF2D::Utilities
{
	inline Core::WaitOneFrame& WaitFrame()
	{
		if (!Core::Task::GetCurrentTask())
		{
			throw std::runtime_error("WaitFrame must be called inside a running Task.");
		}

		auto* awaitable = new Core::WaitOneFrame();

		Core::Task::GetCurrentTask()->AddAwaitable(awaitable);

		return *awaitable;
	}

	inline Core::WaitForSeconds& WaitSeconds(float seconds)
	{
		if (!Core::Task::GetCurrentTask())
		{
			throw std::runtime_error("WaitSeconds must be called inside a running Task.");
		}

		auto* awaitable = new Core::WaitForSeconds(seconds);

		Core::Task::GetCurrentTask()->AddAwaitable(awaitable);

		return *awaitable;
	}


	inline Core::WaitForSecondsUnscaled& WaitSecondsUnscaled(float seconds)
	{
		if (!Core::Task::GetCurrentTask())
		{
			throw std::runtime_error("WaitSeconds must be called inside a running Task.");
		}

		auto* awaitable = new Core::WaitForSecondsUnscaled(seconds);

		Core::Task::GetCurrentTask()->AddAwaitable(awaitable);

		return *awaitable;
	}
}