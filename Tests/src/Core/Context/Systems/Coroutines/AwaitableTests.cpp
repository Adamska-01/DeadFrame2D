#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSeconds.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSecondsUnscaled.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Utilities;


static DF2D::Core::Task FlagTask(bool& flag)
{
	flag = true;

	co_return;
}

static DF2D::Core::Task WaitFrameTask()
{
	co_await CoroutineHelpers::WaitFrame();
}

static DF2D::Core::Task WaitSecondsTask(float seconds)
{
	co_await CoroutineHelpers::WaitSeconds(seconds);
}


TEST_SUITE_BEGIN("Awaitables");


TEST_CASE("WaitOneFrame_Tick_ResumesAndFinishes")
{
	auto resumed = false;

	auto continuation = FlagTask(resumed);

	WaitOneFrame awaitable;
	awaitable.SetHandle(continuation.promiseHandle);

	auto result = awaitable.Tick(0.016f, 0.016f);

	CHECK(result);
	CHECK(resumed);
}


TEST_CASE("WaitOneFrame_Tick_NullHandle_Safe")
{
	WaitOneFrame awaitable;

	auto result = awaitable.Tick(0.016f, 0.016f);

	CHECK(result);
}


TEST_CASE("WaitForSeconds_Tick_CountsDown")
{
	WaitForSeconds awaitable(1.0f);

	auto result = awaitable.Tick(0.3f, 0.0f);

	CHECK_FALSE(result);
}


TEST_CASE("WaitForSeconds_Tick_Finishes")
{
	auto resumed = false;
	auto continuation = FlagTask(resumed);

	WaitForSeconds awaitable(0.5f);
	awaitable.SetHandle(continuation.promiseHandle);

	auto resultTick1 = awaitable.Tick(0.3f, 0.0f);

	CHECK_FALSE(resultTick1);

	auto resultTick2 = awaitable.Tick(0.2f, 0.0f);

	CHECK(resultTick2);
	CHECK(resumed);
}


TEST_CASE("WaitForSeconds_Tick_ExactBoundary")
{
	auto resumed = false;
	auto continuation = FlagTask(resumed);

	WaitForSeconds awaitable(0.3f);
	awaitable.SetHandle(continuation.promiseHandle);

	auto result = awaitable.Tick(0.3f, 0.0f);

	CHECK(result);
	CHECK(resumed);
}


TEST_CASE("WaitForSeconds_awaitReady_PositiveTime")
{
	WaitForSeconds awaitable(1.0f);

	CHECK_FALSE(awaitable.await_ready());
}


TEST_CASE("WaitForSeconds_awaitReady_ZeroTime")
{
	WaitForSeconds awaitable(0.0f);

	CHECK(awaitable.await_ready());
}


TEST_CASE("WaitForSeconds_awaitReady_NegativeTime")
{
	WaitForSeconds awaitable(-1.0f);

	CHECK(awaitable.await_ready());
}


TEST_CASE("WaitForSecondsUnscaled_Tick_CountsDown")
{
	WaitForSecondsUnscaled awaitable(1.0f);

	auto result = awaitable.Tick(0.0f, 0.3f);

	CHECK_FALSE(result);
}


TEST_CASE("WaitForSecondsUnscaled_Tick_Finishes")
{
	bool resumed = false;
	auto continuation = FlagTask(resumed);

	WaitForSecondsUnscaled awaitable(0.5f);
	awaitable.SetHandle(continuation.promiseHandle);

	awaitable.Tick(0.0f, 0.3f);

	auto result = awaitable.Tick(0.0f, 0.2f);

	CHECK(result);
	CHECK(resumed);
}


TEST_CASE("WaitForSecondsUnscaled_Tick_ExactBoundary")
{
	bool resumed = false;
	auto continuation = FlagTask(resumed);

	WaitForSecondsUnscaled awaitable(0.3f);
	awaitable.SetHandle(continuation.promiseHandle);

	auto result = awaitable.Tick(0.0f, 0.3f);

	CHECK(result);
	CHECK(resumed);
}


TEST_CASE("WaitForSecondsUnscaled_awaitReady_PositiveTime")
{
	WaitForSecondsUnscaled awaitable(1.0f);

	CHECK_FALSE(awaitable.await_ready());
}


TEST_CASE("WaitForSecondsUnscaled_awaitReady_ZeroTime")
{
	WaitForSecondsUnscaled awaitable(0.0f);

	CHECK(awaitable.await_ready());
}


TEST_CASE("WaitForSecondsUnscaled_awaitReady_NegativeTime")
{
	WaitForSecondsUnscaled awaitable(-1.0f);

	CHECK(awaitable.await_ready());
}


TEST_CASE("CoroutineAwaitableProxy_AddsAwaitableToTask")
{
	auto task = WaitFrameTask();

	task.promiseHandle.resume();

	CHECK(task.awaitables.size() == 1);
}


TEST_CASE("CoroutineAwaitableProxy_NoSuspendWhenAwaitReady")
{
	auto task = WaitSecondsTask(0.0f);

	task.promiseHandle.resume();

	CHECK(task.awaitables.size() == 0);
	CHECK(task.IsDone());
}


TEST_CASE("CoroutineAwaitableProxy_SuspendsWhenNotReady")
{
	auto task = WaitSecondsTask(1.0f);

	task.promiseHandle.resume();

	CHECK(task.awaitables.size() == 1);
	CHECK_FALSE(task.IsDone());
}


TEST_SUITE_END();
