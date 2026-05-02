#include "Core/Context/Systems/Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Coroutines/Task.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>


namespace DF2D::Core
{
	using namespace DF2D::Utilities;


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

	void CoroutineScheduler::BeginFrame()
	{

	}

	void CoroutineScheduler::PreUpdate(float deltaTime)
	{
		Guard::AgainstNull(instance, NAME_OF(instance));

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

	void CoroutineScheduler::EndUpdate(float deltaTime)
	{

	}

	void CoroutineScheduler::EndDraw()
	{

	}

	Task& CoroutineScheduler::StartCoroutine(Task&& task)
	{
		Guard::AgainstNull(instance, NAME_OF(instance));

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
		Guard::AgainstNull(instance, NAME_OF(instance));

		for (auto* task : instance->tasks)
		{
			delete task;
		}

		instance->tasks.clear();
	}
}