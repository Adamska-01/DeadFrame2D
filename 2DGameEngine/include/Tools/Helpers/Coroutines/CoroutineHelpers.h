#pragma once
#include "Coroutines/CoroutineScheduler.h"
#include "Coroutines/WaitForSeconds.h"
#include "Coroutines/WaitOneFrame.h"


namespace Tools::Helpers::Coroutines
{
	inline auto& WaitFrame() 
	{
		auto awaitable = new WaitOneFrame();

		CoroutineScheduler::AddAwaitable(awaitable);

		return *awaitable;
	}

	inline auto& WaitSeconds(float seconds) 
	{
		auto awaitable = new WaitForSeconds(seconds);
		
		CoroutineScheduler::AddAwaitable(awaitable);
		
		return *awaitable;
	}
}