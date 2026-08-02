#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Coroutines/Task.h"


namespace DF2D::Core
{
	CoroutineScheduler::CoroutineScheduler(const ITimeProvider* timeProvider)
		: timeProvider(timeProvider)
	{
	}

	CoroutineScheduler::~CoroutineScheduler()
	{
		Reset();
	}


	void CoroutineScheduler::BeginFrame()
	{

	}

	void CoroutineScheduler::PreUpdate(float deltaTime)
	{
		auto unscaledDt = timeProvider != nullptr
			? timeProvider->DeltaTimeUnscaled()
			: deltaTime;

		for (auto it = tasks.begin(); it != tasks.end();)
		{
			auto task = *it;

			auto isDoneOrCancelled = task->TickAwaitables(deltaTime, unscaledDt);

			if (isDoneOrCancelled)
			{
				delete task;
				it = tasks.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void CoroutineScheduler::EndUpdate(float deltaTime)
	{

	}

	void CoroutineScheduler::EndDraw()
	{

	}

	void CoroutineScheduler::Reset()
	{
		for (auto* task : tasks)
		{
			delete task;
		}

		tasks.clear();
	}

	Task& CoroutineScheduler::StartCoroutine(Task&& task)
	{
		auto* heapTask = new Task(std::move(task));

		// Now manually resume
		heapTask->promiseHandle.resume();

		tasks.push_back(heapTask);

		return *heapTask;
	}

	size_t CoroutineScheduler::GetActiveTaskCount() const
	{
		return tasks.size();
	}
}