#include "Core/Context/Systems/Coroutines/Awaitables/WaitForSeconds.h"
#include "Core/Context/Systems/Coroutines/Awaitables/WaitOneFrame.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include <doctest.h>


using namespace DF2D::Core;


static DF2D::Core::Task EmptyTask()
{
	co_return;
}

static DF2D::Core::Task FlagTask(bool& flag)
{
	flag = true;

	co_return;
}


TEST_SUITE_BEGIN("Task");


TEST_CASE("Task_Construction")
{
	auto task = EmptyTask();

	CHECK_FALSE(task.IsDone());
	CHECK_FALSE(task.IsCancelled());
	CHECK(task.promiseHandle.promise().owningTask == &task);
}


TEST_CASE("Task_MoveConstructor_UpdatesOwningTask")
{
	auto original = EmptyTask();

	auto moved = std::move(original);

	CHECK(original.promiseHandle == nullptr);
	CHECK(moved.promiseHandle != nullptr);
	CHECK(moved.promiseHandle.promise().owningTask == &moved);
	CHECK_FALSE(moved.IsDone());
}


TEST_CASE("Task_MoveAssignment_DestroysOldAndUpdatesOwningTask")
{
	auto source = EmptyTask();
	auto target = EmptyTask();

	target = std::move(source);

	CHECK(source.promiseHandle == nullptr);
	CHECK(target.promiseHandle != nullptr);
	CHECK(target.promiseHandle.promise().owningTask == &target);
}


TEST_CASE("Task_IsDone_AfterCompletion")
{
	auto task = EmptyTask();

	CHECK_FALSE(task.IsDone());

	task.promiseHandle.resume();

	CHECK(task.IsDone());
}


TEST_CASE("Task_IsCancelled")
{
	auto task = EmptyTask();

	CHECK_FALSE(task.IsCancelled());

	task.Cancel();

	CHECK(task.IsCancelled());
}


TEST_CASE("Task_Cancel_SetsAtomicFlag")
{
	auto task = EmptyTask();

	task.promiseHandle.promise().cancel();

	CHECK(task.promiseHandle.promise().is_cancelled());
}


TEST_CASE("TickAwaitables_TicksMultipleAwaitables")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));
	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));

	auto result = task.TickAwaitables(0.5f, 0.5f);

	CHECK_FALSE(result);
	CHECK(task.awaitables.size() == 2);
}


TEST_CASE("TickAwaitables_RemovesFinishedAwaitables")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));
	task.AddAwaitable(std::make_unique<WaitOneFrame>());
	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));

	CHECK(task.awaitables.size() == 3);

	auto result = task.TickAwaitables(0.016f, 0.016f);

	CHECK_FALSE(result);
	CHECK(task.awaitables.size() == 2);
}


TEST_CASE("TickAwaitables_NewAwaitablesAddedBetweenTicks")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));

	task.TickAwaitables(0.5f, 0.5f);

	CHECK(task.awaitables.size() == 1);

	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));

	task.TickAwaitables(0.5f, 0.5f);

	CHECK(task.awaitables.size() == 1);
}


TEST_CASE("TickAwaitables_Cancelled_SkipsTicking")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitForSeconds>(1.0f));

	task.Cancel();

	auto result = task.TickAwaitables(0.016f, 0.016f);

	CHECK(result);
	CHECK(task.IsCancelled());
}


TEST_CASE("TickAwaitables_AwaitablesFinishedButCoroutineNotDone")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitOneFrame>());
	task.AddAwaitable(std::make_unique<WaitOneFrame>());

	auto result = task.TickAwaitables(0.016f, 0.016f);

	CHECK_FALSE(result);
	CHECK(task.awaitables.size() == 0);
	CHECK_FALSE(task.IsDone());
}


TEST_CASE("TickAwaitables_AwaitablesFinishedAndCoroutineDone")
{
	auto task = EmptyTask();

	task.AddAwaitable(std::make_unique<WaitOneFrame>());
	task.AddAwaitable(std::make_unique<WaitOneFrame>());

	task.promiseHandle.resume();

	auto result = task.TickAwaitables(0.016f, 0.016f);

	CHECK(result);
	CHECK(task.awaitables.size() == 0);
	CHECK(task.IsDone());
}


TEST_CASE("final_awaiter_ResumesContinuation")
{
	auto continuationRan = false;

	auto task = EmptyTask();

	auto continuation = FlagTask(continuationRan);

	task.promiseHandle.promise().continuation = continuation.promiseHandle;

	task.promiseHandle.resume();

	CHECK(continuationRan);
}


TEST_CASE("final_awaiter_NullContinuation_DoesNotCrash")
{
	auto task = EmptyTask();

	task.promiseHandle.promise().continuation = nullptr;

	task.promiseHandle.resume();

	CHECK(task.IsDone());
}


TEST_SUITE_END();
