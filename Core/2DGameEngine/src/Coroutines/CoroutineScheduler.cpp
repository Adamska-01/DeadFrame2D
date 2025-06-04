#include "Coroutines/Abstractions/ICoroutineAwaitable.h"
#include "Coroutines/CoroutineScheduler.h"
#include "Tools/Helpers/Guards.h"
#include <algorithm>
#include <stdexcept>


CoroutineScheduler* CoroutineScheduler::current = nullptr;


CoroutineScheduler::~CoroutineScheduler()
{
	if (current != this)
	{
		throw std::runtime_error("CoroutineScheduler::current does not match this instance.");
	}

	for (auto awaitable : awaitables)
	{
		delete awaitable;
		awaitable = nullptr;
	}

	for (auto task : tasks)
	{
		delete task;
		task = nullptr;
	}

	awaitables.clear();
	tasks.clear();

	current = nullptr;
}

void CoroutineScheduler::AddAwaitable(ICoroutineAwaitable* awaitable)
{
	Tools::Helpers::GuardAgainstNull(current, "CoroutineScheduler::current is null.");

	current->awaitables.push_back(awaitable);
}

void CoroutineScheduler::StartCoroutine(Task&& task)
{
	Tools::Helpers::GuardAgainstNull(current, "CoroutineScheduler::current is null.");

	// Move onto heap so it doesn't go out of scope
	auto* heapTask = new Task(std::move(task));

	current->tasks.emplace_back(heapTask);
}

void CoroutineScheduler::Update(float deltaTime)
{
	Tools::Helpers::GuardAgainstNull(current, "CoroutineScheduler::current is null.");

	// First, collect which awaitables finished
	std::vector<ICoroutineAwaitable*> finished;

	for (auto* awaitable : current->awaitables)
	{
		if (!awaitable->Tick(deltaTime))
			continue;
	
		finished.push_back(awaitable);
	}

	// Now, erase those from the vector (
	for (auto* awaitable : finished)
	{
		auto it = std::find(
			current->awaitables.begin(), 
			current->awaitables.end(), 
			awaitable);

		if (it != current->awaitables.end())
		{
			current->awaitables.erase(it);
		}

		delete awaitable;
	}

	current->tasks.erase(
		std::remove_if(
			current->tasks.begin(), 
			current->tasks.end(),
			[](Task* task) 
			{
				if (task->IsDone())
				{
					delete task;
					task = nullptr;
				
					return true;
				}
			
				return false;
			}),
		current->tasks.end());
}

void CoroutineScheduler::SetCurrent(CoroutineScheduler* scheduler)
{
	current = scheduler;
}