#include "Core/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Core/Coroutines/Task.h"
#include "Core/SubSystems/Systems/CoroutineScheduler.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Utilities;


	CoroutineScheduler* CoroutineScheduler::instance = nullptr;


	CoroutineScheduler::CoroutineScheduler()
	{
		assert(instance == nullptr && "CoroutineScheduler was already initialized!");

		instance = this;
	}

	CoroutineScheduler::~CoroutineScheduler()
	{
		if (instance != this)
		{
			std::cerr << "[Error] CoroutineScheduler::~CoroutineScheduler(): instance mismatch.\n";

			assert(false && "CoroutineScheduler::instance does not match this instance.");
		}

		Reset();

		instance = nullptr;
	}

	void CoroutineScheduler::Update(float deltaTime)
	{
		Guard::AgainstNull(instance, "CoroutineScheduler::current is null.");

		for (auto it = instance->tasks.begin(); it != instance->tasks.end();)
		{
			auto task = *it;

			Task::SetCurrentTask(task);

			auto isDoneOrCancelled = task->TickAwaitables(deltaTime);

			if (isDoneOrCancelled)
			{
				delete task;
				it = instance->tasks.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Reset currentTask after ticking all
		Task::SetCurrentTask(nullptr);
	}

	void CoroutineScheduler::BeginFrame()
	{

	}

	void CoroutineScheduler::EndUpdate()
	{

	}

	void CoroutineScheduler::EndDraw()
	{

	}

	Task& CoroutineScheduler::StartCoroutine(Task&& task)
	{
		Guard::AgainstNull(instance, "CoroutineScheduler::current is null.");

		auto* heapTask = new Task(std::move(task));

		// Set current task so WaitFrame() works during first resume
		Task::SetCurrentTask(heapTask);

		// Now manually resume
		heapTask->promiseHandle.resume();

		// Reset
		Task::SetCurrentTask(nullptr);

		instance->tasks.push_back(heapTask);

		return *heapTask;
	}

	void CoroutineScheduler::Reset()
	{
		Guard::AgainstNull(instance, "CoroutineScheduler::current is null.");

		for (auto* task : instance->tasks)
		{
			delete task;
		}

		instance->tasks.clear();
	}
}