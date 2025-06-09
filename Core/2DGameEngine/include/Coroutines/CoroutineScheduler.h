#pragma once
#include "Coroutines/Task.h"
#include <vector>


class ICoroutineAwaitable;


class CoroutineScheduler 
{
	friend class SubSystems;


private:
	static CoroutineScheduler* current;


	std::vector<ICoroutineAwaitable*> awaitables;
	
	std::vector<Task*> tasks;
	

	CoroutineScheduler() = default;

	~CoroutineScheduler();


	void Update(float deltaTime);

	
	static void SetCurrent(CoroutineScheduler* scheduler);


public:
	CoroutineScheduler(const CoroutineScheduler&) = delete;

	CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;


	static void AddAwaitable(ICoroutineAwaitable* awaitable);

	static Task& StartCoroutine(Task&& task);
};