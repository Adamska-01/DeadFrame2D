#pragma once
#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSeconds.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSecondsUnscaled.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"
#include "Core/Context/Systems/Coroutines/CoroutineAwaitableProxy.h"
#include <tuple>


namespace DF2D::Utilities::CoroutineHelpers
{
	inline auto WaitFrame()
	{
		return Core::MakeAwaitableProxy<Core::WaitOneFrame>();
	}

	inline auto WaitSeconds(float seconds)
	{
		return Core::MakeAwaitableProxy<Core::WaitForSeconds>(seconds);
	}

	inline auto WaitSecondsUnscaled(float seconds)
	{
		return Core::MakeAwaitableProxy<Core::WaitForSecondsUnscaled>(seconds);
	}
}