#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Utilities;


static Task EmptyTask()
{
	co_return;
}

static Task FlagTask(bool& flag)
{
	flag = true;

	co_return;
}

static Task WaitFrameTask(bool& resumed)
{
	co_await CoroutineHelpers::WaitFrame();

	resumed = true;
}

static Task WaitFrameTask()
{
	co_await CoroutineHelpers::WaitFrame();
}

static Task WaitSecondsTask(float seconds, bool& resumed)
{
	co_await CoroutineHelpers::WaitSeconds(seconds);

	resumed = true;
}

static Task WaitSecondsTask(float seconds)
{
	co_await CoroutineHelpers::WaitSeconds(seconds);
}


TEST_SUITE_BEGIN("CoroutineScheduler");


TEST_CASE("StartCoroutine_ImmediatelyResumesCoroutine")
{
	auto flag = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(FlagTask(flag));

	CHECK(flag);
}


TEST_CASE("PreUpdate_RemovesDoneTasks")
{
	auto resumed = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitFrameTask(resumed));

	CHECK_FALSE(resumed);

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);

	CHECK(resumed);
}


TEST_CASE("PreUpdate_RemovesCancelledTasks")
{
	auto resumed = false;

	CoroutineScheduler scheduler;

	auto& task = scheduler.StartCoroutine(WaitSecondsTask(10.0f, resumed));

	task.Cancel();

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);

	CHECK_FALSE(resumed);
}


TEST_CASE("PreUpdate_HandlesMultipleTasks")
{
	auto resumed1 = false;
	auto resumed2 = false;
	auto resumed3 = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitFrameTask(resumed1));
	scheduler.StartCoroutine(WaitFrameTask(resumed2));
	scheduler.StartCoroutine(WaitFrameTask(resumed3));

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);

	CHECK(resumed1);
	CHECK(resumed2);
	CHECK(resumed3);
}


TEST_CASE("PreUpdate_WaitForSeconds_RespectsScaledDt")
{
	auto resumed = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitSecondsTask(1.0f, resumed));

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.5f);

	CHECK_FALSE(resumed);

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.5f);

	CHECK(resumed);
}


TEST_CASE("PreUpdate_WaitForSeconds_FinishesInSingleTick")
{
	bool resumed = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitSecondsTask(1.0f, resumed));

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(1.5f);

	CHECK(resumed);
}


TEST_CASE("Destructor_CleansUpAllTasks")
{
	{
		CoroutineScheduler scheduler;

		scheduler.StartCoroutine(WaitSecondsTask(10.0f));
		scheduler.StartCoroutine(WaitSecondsTask(10.0f));
	}

	CHECK(true);
}


TEST_CASE("StartCoroutine_WaitFrameTask_CompletesInOnePreUpdate")
{
	auto resumed = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitFrameTask(resumed));

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);

	CHECK(resumed);

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);
}


TEST_CASE("StartCoroutine_EmptyTask_CompletesImmediately")
{
	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(EmptyTask());

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);
}


TEST_CASE("PreUpdate_DoneTasksAreRemoved")
{
	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitFrameTask());

	CHECK_EQ(scheduler.GetActiveTaskCount(), 1); // started

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.016f);

	CHECK_EQ(scheduler.GetActiveTaskCount(), 0); // removed
}


TEST_CASE("PreUpdate_PendingTasksRemainAfterTick")
{
	bool resumed = false;

	CoroutineScheduler scheduler;

	scheduler.StartCoroutine(WaitSecondsTask(1.0f, resumed));

	CHECK_EQ(scheduler.GetActiveTaskCount(), 1);

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.5f);

	CHECK_FALSE(resumed);

	static_cast<ICoreSystem*>(&scheduler)->PreUpdate(0.5f);

	CHECK(resumed);
	CHECK_EQ(scheduler.GetActiveTaskCount(), 0);
}


TEST_SUITE_END();
