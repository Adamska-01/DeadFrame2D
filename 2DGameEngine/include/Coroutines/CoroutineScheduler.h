#pragma once
#include "Coroutines/Task.h"
#include "SubSystems/Abstractions/ISubSystem.h"
#include <vector>


class ICoroutineAwaitable;


class CoroutineScheduler : public ISubSystem
{
	friend class SubSystems;


private:
	static CoroutineScheduler* current;


	std::vector<ICoroutineAwaitable*> awaitables;
	
	std::vector<Task*> tasks;
	

	CoroutineScheduler() = default;

	virtual ~CoroutineScheduler() override;


	virtual void Update(float deltaTime) override;

	virtual void BeginFrame() override;

	virtual void EndUpdate() override;

	virtual void EndDraw() override;


	static void SetCurrent(CoroutineScheduler* scheduler);


public:
	CoroutineScheduler(const CoroutineScheduler&) = delete;

	CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;


	static void AddAwaitable(ICoroutineAwaitable* awaitable);

	static Task& StartCoroutine(Task&& task);

	static void Reset();
};